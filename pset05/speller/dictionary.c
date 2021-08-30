// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100000;

// Number of words in dictionary
int wordCount = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *selected = table[hash(word)];
    while (true)
    {
        if (selected == NULL)
        {
            break;
        }
        if (strcasecmp(selected->word, word) == 0)
        {
            return true;
        }
        selected = selected->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //---HASH ALGORITHM FROM AUSTIN TACKABERRY---
    long sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    return sum % N;
    //----------
    //return (word[0] - 97);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *words = fopen(dictionary, "r");
    if (words == NULL)
    {
        return false;
    }
    char word[LENGTH + 1];
    while (fscanf(words, "%s", word) != EOF)
    {
        wordCount++;
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, word);
        n->next = table[hash(word)];
        table[hash(word)] = n;
    }
    fclose(words);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return wordCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *selected = table[i];
        while (true)
        {
            if (selected == NULL)
            {
                break;
            }
            node *deleteSelected = selected;
            selected = selected->next;
            free(deleteSelected);
        }
    }
    return true;
}
