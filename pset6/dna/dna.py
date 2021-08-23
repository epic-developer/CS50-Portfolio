import csv
import sys


def main():
    if (len(sys.argv) < 3):
        print("Usage: python dna.py data.csv sequence.txt")
        exit()
    STRs = {}
    people = {}
    with open(sys.argv[2]) as txtfile:
        lines = txtfile.readlines()
        sequence = lines[0]
    with open(sys.argv[1], newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=' ', quotechar='|')
        fileSTRs = []
        fileNames = []
        for row in reader:
            row = row[0].split(',')
            if (row[0] == 'name'):
                for item in row:
                    if item != 'name':
                        fileSTRs.append(item)
                continue
            i = 1
            fileNames.append(row[0])
            people[row[0]] = {}
            for STR in fileSTRs:
                people[row[0]][STR] = int(row[i])
                i += 1
        for STR in fileSTRs:
            STRs[STR] = checkSTR(STR, sequence)
        print(findProfile(people, STRs, fileSTRs, fileNames))


def checkSTR(STR, sequence):
    strOccurances = []
    length = len(sequence)
    pos = 0
    n = len(STR)
    while (pos < length):
        if (sequence[pos:(pos + n)] == STR):
            strOccurances.append(findSTR(STR, sequence[pos:length], n))
        pos += 1
    strOccurances.sort(reverse=True)
    if (len(strOccurances) == 0):
        strOccurances.append(0)
    return strOccurances[0]


def findSTR(STR, sequence, n):
    if STR == sequence[0:n]:
        return findSTR(STR, sequence[n:len(sequence)], n) + 1
    else:
        return 0


def findProfile(profiles, STRs, checks, names):
    for name in names:
        check = True
        for STR in checks:
            if (profiles[name][STR] != STRs[STR]):
                check = False
                break
        if (check == True):
            return name
    return 'No Match'


main()