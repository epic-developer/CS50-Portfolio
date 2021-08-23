import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        if (name == None or month == None or day == None or name == "" or month == "" or day == ""):
            return "Please enter a name, month, and day."
        '''
        if (type(name) is not type("") or type(month) is not type(0) or type(day) is not type(0)):
            return "Name, month, and/or day invalid."
        '''
        month = int(month)
        day = int(day)
        if (len(name) > 20):
            return "Name must be less than 20 characters long."
        elif (month > 12 or month < 1):
            return "Month invalid. Please select a value of 1 through 12."
        elif (day > 31 or day < 1):
            return "Day invalid. Please select a value of 1 through 31."
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")

    else:

        # Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")

        return render_template("index.html", birthdays=birthdays)