from cs50 import get_string


def evaluate(txt):
    words, letters, sentences = 0, 0, 0
    txtLength = len(txt)
    i = 0
    txt.upper()
    while (i < txtLength):
        if (txt[i] == ' '):
            words += 1
        elif (txt[i] == '.' or txt[i] == '!' or txt[i] == '?'):
            sentences += 1
        elif txt[i].isalpha():
            letters += 1
        i += 1
    words += 1
    return round(0.0588 * ((letters / words) * 100) - 0.296 * ((sentences / words) * 100) - 15.8)


text = get_string("Text: ")
grade = evaluate(text)

if (grade < 1):
    print("Before Grade 1")
elif (grade >= 16):
    print("Grade 16+")
else:
    print("Grade " + str(grade))