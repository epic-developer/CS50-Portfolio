#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int getLevel(string txt);

int main(void)
{
    string text = get_string("Text: ");
    int grade = getLevel(text);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int getLevel(string txt)
{
    double words = 0, letters = 0, sentences = 0;
    for (int i = 0, txtLength = strlen(txt); i < txtLength; i++)
    {
        txt[i] = toupper(txt[i]);
        if (txt[i] == ' ')
        {
            words++;
        }
        else if (txt[i] == '.' || txt[i] == '!' || txt[i] == '?')
        {
            sentences++;
        }
        else if (txt[i] >= 'A' && txt[i] <= 'Z')
        {
            //printf("%c", txt[i]);
            letters++;
        }
    }
    words++;
    /*
    printf("\n%f Letter(s)", letters);
    printf("\n%f Word(s)", words);
    printf("\n%f Sentence(s)\n", sentences);
    printf("\n%f Letters per 100 words\n", letters / (words / 100));
    printf("\n%f Letters per 100 words\n", sentences / (words / 100));
    */
    int grade = round(0.0588 * (letters / (words / 100)) - 0.296 * (sentences / (words / 100)) - 15.8);
    return grade;
}