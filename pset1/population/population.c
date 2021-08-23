#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt user:
    int startSize;
    do
    {
        startSize = get_int("Enter the population start size: ");
    }
    while (startSize < 9);

    int endSize;
    do
    {
        endSize = get_int("Enter the population end size: ");
    }
    while (endSize < startSize); //Change to <= to ensure change


    //Loop through until (current size == end size)
    int years = 0;
    int currentSize = startSize;
    while (currentSize < endSize)
    {
        currentSize = (currentSize + (currentSize / 3)) - (currentSize / 4); // n/3 llamas born and n/4 llamas died
        years++; // one year passed
    }
    //printf("\n\nIt took %i years for the population to grow from %i to %i.\n", years, startSize, currentSize); //display results
    printf("\nYears: %d", years);
}