#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    BYTE buffer[512];
    int counter = 0;
    FILE *writingJPEG = NULL;
    char filename[8];
    while (fread(&buffer, 512, 1, file) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (counter > 0)
            {
                //Close previous JPEG if needed
                fclose(writingJPEG);
            }
            sprintf(filename, "%03i.jpg", counter); //Use 3 digits for filename, regardless of counter value
            writingJPEG = fopen(filename, "w");
            counter++;
        }
        if (counter > 0)
        {
            //If a file is open
            fwrite(&buffer, 512, 1, writingJPEG);
        }
    }
    fclose(file);
    fclose(writingJPEG);
    return 0;
}