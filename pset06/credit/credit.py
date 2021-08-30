from cs50 import get_int

cn = str(get_int("Number: "))

if (len(cn) != 13 and len(cn) != 15 and len(cn) != 16):
    print("INVALID")
    exit()

i = 0

total = 0

# --- Does not work with odd numbers
'''
while (i < len(cn)):
    if (i % 2 == 0):
        product = str(int(cn[i]) * 2)
        x = 0
        while (x < len(product)):
            total += int(product[x])
            x += 1
    else:
        total += int(cn[i])
    i += 1
'''
# ---

# Adapted from https://stackoverflow.com/questions/63322643/cs50-python-credit-not-working-for-amex-working-fine-with-other-cards


def cardSum(digits, parity):
    if not digits:
        return 0

    digit, digits = int(digits[-1]), digits[:-1]

    if len(digits) % 2 == parity:
        digit *= 2
        digit = digit // 10 + digit % 10

    return digit + cardSum(digits, parity)


length = len(cn)
total = str(cardSum(cn, length % 2))
first = int(cn[0])
first2 = int(cn[0] + cn[1])

if (total[len(total) - 1] != "0"):
    print("INVALID")
    exit()

if ((length == 13 or length == 16) and first == 4):
    print("VISA")
elif (length == 16 and (first2 == 51 or first2 == 52 or first2 == 53 or first2 == 54 or first2 == 55)):
    print("MASTERCARD")
elif (length == 15 and (first2 == 34 or first2 == 37)):
    print("AMEX")
else:
    print("INVALID")