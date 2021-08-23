import os

from cs50 import SQL
import datetime
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
os.environ["API_KEY"] = "pk_64a13dd07500485f9f923c5d0412fe0b"
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    netBalance = balance
    stocks = db.execute("SELECT * FROM holdings WHERE type = 'BUY' AND user = ?", session["user_id"])
    sales = db.execute("SELECT * FROM holdings WHERE type = 'SELL' AND user = ?", session["user_id"])
    holdings = {}
    tickers = []
    for stock in stocks:
        symbol = stock["stock"]
        if symbol not in holdings:
            moreInfo = lookup(symbol)
            holdings[symbol] = {
                "symbol": moreInfo["symbol"],
                "name": moreInfo["name"],
                "shares": 0,
                "price": moreInfo["price"],
                "total": 0
            }
            tickers.append(symbol)
        holdings[symbol]["shares"] += stock["shares"]
    for sale in sales:
        symbol = sale["stock"]
        if symbol not in holdings:
            continue
        holdings[symbol]["shares"] -= sale["shares"]
        if (holdings[symbol]["shares"] <= 0):
            del holdings[symbol]
            tickers.remove(symbol)
    holdingsArray = []
    for ticker in tickers:
        holding = holdings[ticker]
        holding["total"] = holding["shares"] * holding["price"]
        netBalance += holding["total"]
        holdingsArray.append(holding)
    return render_template("index.html", holdings=holdingsArray, balance=balance, netBalance=netBalance)


@app.route("/stockLookup", methods=["GET", "POST"])
@login_required
def stockLookup():
    """Lookup Stock"""
    if request.method == "POST":
        if not lookup(request.form.get("symbol")):
            return apology("Stock not found. The ticker symbol entered may be invalid.")
        return render_template("stockInfo.html", stockSymbol=request.form.get("symbol").upper())
    else:
        return render_template("stockSearch.html")


@app.route("/addCash", methods=["GET", "POST"])
@login_required
def addCash():
    """Add money to account"""
    if request.method == "POST":
        amount = int(request.form.get("amount"))
        if (not amount or amount < 20):
            return apology("You must add at least 20 USD to your account", 400)
        db.execute("UPDATE users SET cash = cash + ?", amount)
        return redirect("/")
    else:
        return render_template("addCash.html")


def ValidateInt(string):
    if not string or len(string) == 0 or "." in string or "-" in string:
        return False
    try:
        int(string)
        return True
    except ValueError:
        return False


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")
        if not ValidateInt(shares):
            return apology("You must enter a positive integer for the number of shares", 400)
        shares = int(shares)
        if shares < 1:
            return apology("You must purchase at least 1 share", 400)
        if not stock:
            return apology("must provide a valid stock ticker symbol", 400)
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        stockPrice = stock["price"]
        buyPrice = stockPrice * shares
        if (userCash < buyPrice):
            return apology(f"You need {str(buyPrice - userCash)} more USD purchase {str(shares)} share(s)", 400)
        db.execute("UPDATE users SET cash = ?", userCash - buyPrice)
        db.execute("INSERT INTO holdings (user, stock, shares, price, time, type) VALUES(?, ?, ?, ?, datetime('now'), 'BUY')",
                   session["user_id"], stock["symbol"], shares, stockPrice)
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM holdings WHERE user = ?", session["user_id"])
    return render_template("history.html", logs=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        stock = request.form.get("symbol")
        if not stock:
            return apology("must provide a stock ticker symbol", 400)
        stock = lookup(stock)
        if not stock:
            return apology("must provide a valid stock ticker symbol", 400)
        return render_template("quoted.html", name=stock["name"], price=stock["price"], symbol=stock["symbol"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username:
            return apology("must provide username", 400)
        elif not password:
            return apology("must provide password", 400)
        elif not confirmation:
            return apology("must provide password confirmation", 400)
        elif password != confirmation:
            return apology("password must match password confirmation", 400)
        if (len(db.execute("SELECT cash FROM users WHERE username = ?", username)) > 0):
            return apology("account with username already exists", 400)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username,
                   generate_password_hash(password, method='pbkdf2:sha256', salt_length=8))
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        if (not request.form.get("symbol")):
            return apology("select a valid ticker symbol", 400)
        stock = lookup(request.form.get("symbol"))
        if (not stock):
            return apology("select a valid ticker symbol", 400)
        amount = request.form.get("shares")
        if not ValidateInt(amount):
            return apology("You must enter a positive integer for the number of shares", 400)
        amount = int(amount)
        if amount < 1:
            return apology("You must sell at least 1 share", 400)
        shares = db.execute("SELECT SUM(shares) AS shares FROM holdings WHERE user = ? AND stock = ?",
                            session["user_id"], stock["symbol"])[0]["shares"]
        if amount > shares:
            return apology("you are trying to sell more shares than you own", 400)
        db.execute("UPDATE users SET cash = cash + ?", amount * stock["price"])
        db.execute("INSERT INTO holdings (user, stock, shares, price, time, type) VALUES(?, ?, ?, ?, datetime('now'), 'SELL')",
                   session["user_id"], stock["symbol"], amount, stock["price"])
        return redirect("/")
    else:
        symbols = db.execute("SELECT stock FROM holdings WHERE user = ? GROUP BY stock", session["user_id"])
        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)