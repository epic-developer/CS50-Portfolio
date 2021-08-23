#include "helpers.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            RGBTRIPLE pixel = image[y][x];
            float r = image[y][x].rgbtRed;
            float g = image[y][x].rgbtGreen;
            float b = image[y][x].rgbtBlue;
            int a = round((r + g + b) / 3);
            image[y][x].rgbtRed = a;
            image[y][x].rgbtGreen = a;
            image[y][x].rgbtBlue = a;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0, width2 = width / 2; x < width2; x++)
        {
            RGBTRIPLE rightPixel = image[y][x];
            image[y][x] = image[y][width - (x + 1)]; //Replace right pixel with left pixel
            image[y][width - (x + 1)] = rightPixel; //Replace left pixel with right pixel
        }
    }
    return;
}

void reflect2(int height, int width, RGBTRIPLE image[height][width])
{
    for (int y = 0; y < height; y++)
    {
        RGBTRIPLE tempRow[height][width];
        for (int x = 0; x < width; x++)
        {
            tempRow[y][x] = image[y][x];
        }
        for (int x = 0; x < width; x++)
        {
            image[y][x] = tempRow[y][width - x + 1];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            tempImage[y][x] = image[y][x];
        }
    }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //Get average of color values of 9 pixels (8 surrounding pixels and this pixel)
            int pixelCount = 0;
            float aRed = 0;
            float aGreen = 0;
            float aBlue = 0;
            for (int y2 = -1; y2 < 2; y2++)
            {
                int yCoord = y + y2;
                if (yCoord < 0 || yCoord + 1 > height)
                {
                    continue;
                }
                for (int x2 = -1; x2 < 2; x2++)
                {
                    int xCoord = x + x2;
                    if (xCoord < 0 || xCoord + 1 > width)
                    {
                        continue;
                    }
                    RGBTRIPLE selectedPixel = tempImage[yCoord][xCoord];
                    aRed += selectedPixel.rgbtRed;
                    aGreen += selectedPixel.rgbtGreen;
                    aBlue += selectedPixel.rgbtBlue;
                    pixelCount++;
                }
            }
            image[y][x].rgbtRed = round(aRed / pixelCount);
            image[y][x].rgbtGreen = round(aGreen / pixelCount);
            image[y][x].rgbtBlue = round(aBlue / pixelCount);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            tempImage[y][x] = image[y][x];
        }
    }
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //Get average of color values of 9 pixels (8 surrounding pixels and this pixel)
            int pixelCount = 0;
            float rGx = 0;
            float gGx = 0;
            float bGx = 0;
            float rGy = 0;
            float gGy = 0;
            float bGy = 0;
            for (int y2 = -1; y2 < 2; y2++)
            {
                int yCoord = y + y2;
                if (yCoord < 0 || yCoord + 1 > height)
                {
                    continue;
                }
                for (int x2 = -1; x2 < 2; x2++)
                {
                    int xCoord = x + x2;
                    if (xCoord < 0 || xCoord + 1 > width)
                    {
                        continue;
                    }
                    RGBTRIPLE selectedPixel = tempImage[yCoord][xCoord];
                    int gIndex1 = y2 + 1;
                    int gIndex2 = x2 + 1;
                    rGx += (Gx[gIndex1][gIndex2] * selectedPixel.rgbtRed);
                    gGx += (Gx[gIndex1][gIndex2] * selectedPixel.rgbtGreen);
                    bGx += (Gx[gIndex1][gIndex2] * selectedPixel.rgbtBlue);
                    rGy += (Gy[gIndex1][gIndex2] * selectedPixel.rgbtRed);
                    gGy += (Gy[gIndex1][gIndex2] * selectedPixel.rgbtGreen);
                    bGy += (Gy[gIndex1][gIndex2] * selectedPixel.rgbtBlue);
                    pixelCount++;
                }
            }
            //Compute each new channel value
            int rCv = round(sqrt((rGx * rGx) + (rGy * rGy))); //Red channel value
            int gCv = round(sqrt((gGx * gGx) + (gGy * gGy))); //Green channel value
            int bCv = round(sqrt((bGx * bGx) + (bGy * bGy))); //Blue channel value
            //Cap channel values at 255 as Brian described that it could be greater than 255
            if (rCv > 255)
            {
                //Cap red
                rCv = 255;
            }
            if (gCv > 255)
            {
                //Cap green
                gCv = 255;
            }
            if (bCv > 255)
            {
                //Cap blue
                bCv = 255;
            }
            image[y][x].rgbtRed = rCv;
            image[y][x].rgbtGreen = gCv;
            image[y][x].rgbtBlue = bCv;
        }
    }
    return;
}