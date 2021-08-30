from cs50 import get_int

while (True):
    amount = get_int("Height: ")
    if (amount > 0 and amount < 9):
        break

i = 1

while (i <= amount):
    print((" " * (amount - i)) + ("#" * i) + "  " + ("#" * i))
    i += 1