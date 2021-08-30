#include <cs50.h>
#include <stdio.h>
#include <math.h>

bool verifyCN(long n); //Prototype
string identifyCN(long n); //Prototype

int main(void)
{
    //Get credit card number with prompt to user
    long cn;
    bool check;
    cn = get_long("Number: ");
    check = verifyCN(cn);
    if (check)
    {
        printf("%s\n", identifyCN(cn));
    }
    else
    {
        printf("INVALID\n");
    }
}

bool verifyCN(long n)
{
    //Checksum
    //---
    int c1 = 0;
    int c2 = 0;
    long mod = 10;
    int iteration = 1; //Just have to iterate from one for purposes you can find later
    while (mod <= (n * 10))
    {
        long digit = (n % mod) / (mod / 10);
        if (iteration % 2 == 0)
        {
            int digit2 = digit * 2;
            c1 += (digit2 % 10);
            c1 += ((digit2 % 100) / 10);
        }
        else
        {
            c2 += digit;
        }
        mod *= 10;
        iteration++;
    }
    if ((c1 + c2) % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
    //---
}

string identifyCN(long n)
{
    if ((n >= 1000000000000000 || n >= 1000000000000) && ((((n % 10000000000000000) / 1000000000000000) == 4)
            || (((n % 10000000000000) / 1000000000000) == 4))) //13 or 16 Digits (starts with 4): VISA
    {
        return "VISA";
    }
    else if (n >= 1000000000000000) //16 Digits: Mastercard
    {
        long cNum = n;
        if (cNum % 100000000000000 / 10000000000 == 51)
        {
            cNum = cNum % 100000000000000 / 100000000000;
        }
        else
        {
            cNum = cNum % 10000000000000 / 100000000000;
        }
        switch (cNum)
        {
            case 51:
                break;
            case 52:
                break;
            case 53:
                break;
            case 54:
                break;
            case 55:
                break;
            default:
                return "INVALID";

        }
        return "MASTERCARD";
    }
    else if (n >= 100000000000000 && n <= 1000000000000000 && (((n % 1000000000000000) / 10000000000000) == 34
             || ((n % 1000000000000000) / 10000000000000) == 37)) //15 Digits: American Express
    {
        return "AMEX";
    }
    else
    {
        return "INVALID";
    }
}