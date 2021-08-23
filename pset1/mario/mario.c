#include <cs50.h>
#include <stdio.h>

void genLine(int n);
void genLine2(int n);

int main(void)
{
    //Get height with prompt to user
    int size;
    do
    {
        size = get_int("Height: ");
    }
    while (size < 1 || size > 8);
    //Render pyramid
    int n = 1; //Loop through each row
    while (n <= size)
    {
        genLine2((size - 1) - n); //Generates spacing on side of pyramid
        genLine(n - 1); //Generates right side of pyramid
        printf("  "); //Generates middle of pyramid
        genLine(n - 1); //Generates left side of pyramid
        printf("\n"); //Time for next line of pyramid
        n++;
    }
}

void genLine(int n)
{
    int x = 0;
    while (x <= n)
    {
        printf("#");
        x++;
    }
}

void genLine2(int n)
{
    int x = 0;
    while (x <= n)
    {
        printf(" ");
        x++;
    }
}