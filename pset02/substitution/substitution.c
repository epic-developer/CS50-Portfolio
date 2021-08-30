#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

string encrypt(string key, string txt);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    string cipher = argv[1];
    if (strlen(cipher) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        char checked[26] = "";
        for (int i = 0; i < 26; i++)
        {
            if ((!((cipher[i] >= 'A' && cipher[i] <= 'Z') || (cipher[i] >= 'a' && cipher[i] <= 'z')))
                || (strchr(checked, toupper(cipher[i])) != NULL))
            {
                printf("Key must contain 26 characters.\n");
                return 1;
            }
            checked[i] = toupper(cipher[i]);
        }
    }
    string text = get_string("plaintext: ");
    printf("ciphertext: %s\n", encrypt(cipher, text));
    return 0;
}

string encrypt(string key, string txt)
{
    char key1[26] = ""; //Lowercase cipher key
    char key2[26] = ""; //Uppercase cipher key
    for (int i = 0, keyLength = strlen(key); i < keyLength; i++)
    {
        key1[i] = tolower(key[i]);
        key2[i] = toupper(key[i]);
    }
    for (int i = 0, txtLength = strlen(txt); i < txtLength; i++)
    {
        if (txt[i] >= 'a' && txt[i] <= 'z')
        {
            txt[i] = key1[txt[i] - 97];
        }
        else if (txt[i] >= 'A' && txt[i] <= 'Z')
        {
            txt[i] = key2[txt[i] - 65];
        }
    }
    return txt;
}