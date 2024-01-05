#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE average = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int width_range = width / 2;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width_range; j++)
        {
            // allocating temporary space for pixels on the left side
            BYTE *temp_RGB = malloc(3 * sizeof(BYTE));
            temp_RGB[0] = image[i][j].rgbtRed;
            temp_RGB[1] = image[i][j].rgbtGreen;
            temp_RGB[2] = image[i][j].rgbtBlue;

            // copying the right side to the left side
            image[i][j].rgbtRed = image[i][width - (j + 1)].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - (j + 1)].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - (j + 1)].rgbtBlue;

            // copying the temp to the right side
            image[i][width - (j + 1)].rgbtRed = temp_RGB[0];
            image[i][width - (j + 1)].rgbtGreen = temp_RGB[1];
            image[i][width - (j + 1)].rgbtBlue = temp_RGB[2];

            // freeing the temporary space
            free(temp_RGB);
        }
    }

    return;
}

// fuction which either blurs or find edges
void change_image(int height, int width, RGBTRIPLE image[height][width], int func)
{
    // allocates two rows worth of temp memory
    RGBTRIPLE *row_1 = malloc(width * sizeof(RGBTRIPLE));
    RGBTRIPLE *row_2 = malloc(width * sizeof(RGBTRIPLE));

    // iterates over all rows
    for (int i = 0; i < height; i++)
    {
        // iterates over all columns
        for (int j = 0; j < width; j++)
        {
            BYTE *pixels_Red = malloc(9 * sizeof(BYTE));
            BYTE *pixels_Green = malloc(9 * sizeof(BYTE));
            BYTE *pixels_Blue = malloc(9 * sizeof(BYTE));
            int pixels = 0;
            int i_missed = 0;
            int j_missed = 0;

            // chooses x values from 1 below to 1 above
            for (int y = -1; y < 2; y++)
            {
                if ((y + i > -1) && (y + i < height))
                {
                    // chooses y values from 1 below to 1 above
                    for (int x = -1; x < 2; x++)
                    {
                        if ((x + j > -1) && (x + j < width))
                        {
                            // adds selected pixel info to sums
                            pixels_Red[pixels] = image[y + i][x + j].rgbtRed;
                            pixels_Green[pixels] = image[y + i][x + j].rgbtGreen;
                            pixels_Blue[pixels] = image[y + i][x + j].rgbtBlue;
                            pixels += 1;
                        }
                        else
                        {
                            j_missed = x;
                        }
                    }
                }
                else
                {
                    i_missed = y;
                }
            }

            float sum_Red = 0;
            float sum_Green = 0;
            float sum_Blue = 0;

            if (func == 1)
            {
                // calculates the cumalative sum of blurred pixels
                for (int current_pix = 0; current_pix < pixels; current_pix++)
                {
                    sum_Red += pixels_Red[current_pix];
                    sum_Green += pixels_Green[current_pix];
                    sum_Blue += pixels_Blue[current_pix];
                }

                // allocates blurred pixel to one of the temp rows dependant on i value
                if (i % 2 == 0)
                {
                    row_1[j].rgbtRed = round(sum_Red / pixels);
                    row_1[j].rgbtGreen = round(sum_Green / pixels);
                    row_1[j].rgbtBlue = round(sum_Blue / pixels);
                }
                else
                {
                    row_2[j].rgbtRed = round(sum_Red / pixels);
                    row_2[j].rgbtGreen = round(sum_Green / pixels);
                    row_2[j].rgbtBlue = round(sum_Blue / pixels);
                }
            }
            else if (func == 2)
            {
                float GxRed = 0;
                float GyRed = 0;
                float GxGreen = 0;
                float GyGreen = 0;
                float GxBlue = 0;
                float GyBlue = 0;

                // runs through captured pixel values
                for (int current_pix = 0; current_pix < pixels; current_pix++)
                {
                    // ensures the right pixel is used
                    int used_pix = current_pix;

                    // when on left side edge
                    if (j_missed < 0)
                    {
                        if (used_pix < 2)
                        {
                            used_pix += 1;
                        }
                        else if (used_pix < 4)
                        {
                            used_pix += 2;
                        }
                        else if (used_pix < 6)
                        {
                            used_pix += 3;
                        }
                    }
                    // when on right side edge
                    else if (j_missed > 0)
                    {
                        if (used_pix > 1 && used_pix < 4)
                        {
                            used_pix += 1;
                        }
                        else if (used_pix > 3)
                        {
                            used_pix += 2;
                        }
                    }

                    // when on top edge
                    if (i_missed < 0)
                    {
                        used_pix += 3;
                    }

                    float modifier = 1;

                    // Gx
                    if (used_pix < 3 || used_pix > 5)
                    {
                        modifier = 1;
                    }
                    else
                    {
                        modifier = 2;
                    }

                    if (used_pix % 3 == 0)
                    {
                        modifier *= -1;
                    }
                    else if ((used_pix - 1) % 3 == 0)
                    {
                        modifier *= 0;
                    }

                    GxRed += modifier * pixels_Red[current_pix];
                    GxGreen += modifier * pixels_Green[current_pix];
                    GxBlue += modifier * pixels_Blue[current_pix];

                    // Gy
                    if ((used_pix - 1) % 3 == 0)
                    {
                        modifier = 2;
                    }
                    else
                    {
                        modifier = 1;
                    }

                    if (used_pix < 3)
                    {
                        modifier *= -1;
                    }
                    else if (used_pix < 6)
                    {
                        modifier *= 0;
                    }

                    // adds this pixel to the colour totals
                    GyRed += modifier * pixels_Red[current_pix];
                    GyGreen += modifier * pixels_Green[current_pix];
                    GyBlue += modifier * pixels_Blue[current_pix];
                }

                // finishes the Sobel algorithm by combining Gx and Gy
                sum_Red = sqrtf((GxRed * GxRed) + (GyRed * GyRed));
                sum_Green = sqrtf((GxGreen * GxGreen) + (GyGreen * GyGreen));
                sum_Blue = sqrtf((GxBlue * GxBlue) + (GyBlue * GyBlue));

                // ensures maximum BYTE isn't exceeded
                if (sum_Red > 255)
                {
                    sum_Red = 255;
                }

                if (sum_Green > 255)
                {
                    sum_Green = 255;
                }

                if (sum_Blue > 255)
                {
                    sum_Blue = 255;
                }

                // allocates pixel to one of the temp rows dependant on i value
                if (i % 2 == 0)
                {
                    row_1[j].rgbtRed = round(sum_Red);
                    row_1[j].rgbtGreen = round(sum_Green);
                    row_1[j].rgbtBlue = round(sum_Blue);
                }
                else
                {
                    row_2[j].rgbtRed = round(sum_Red);
                    row_2[j].rgbtGreen = round(sum_Green);
                    row_2[j].rgbtBlue = round(sum_Blue);
                }
            }

            // frees the allocated memory
            free(pixels_Red);
            free(pixels_Green);
            free(pixels_Blue);

        }

        // tells function when to replace row with a temp row
        if (i != 0)
        {
            int repeats = 1;

            // ensures the final row is included
            if (i == height - 1)
            {
                repeats = 2;
            }

            // only adds one row each height iteration apart from the final one where it adds two
            for (int iteration = 0; iteration < repeats; iteration++)
            {
                // temp row added depending on which row still needs to be used for blurring
                if (i % 2 == 0)
                {
                    for (int j = 0; j < width; j++)
                    {
                        image[i - 1][j].rgbtRed = row_2[j].rgbtRed;
                        image[i - 1][j].rgbtGreen = row_2[j].rgbtGreen;
                        image[i - 1][j].rgbtBlue = row_2[j].rgbtBlue;
                    }
                }
                else
                {
                    for (int j = 0; j < width; j++)
                    {
                        image[i - 1][j].rgbtRed = row_1[j].rgbtRed;
                        image[i - 1][j].rgbtGreen = row_1[j].rgbtGreen;
                        image[i - 1][j].rgbtBlue = row_1[j].rgbtBlue;
                    }
                }

                // increases the height by one on the final iteration
                if (repeats == 2)
                {
                    i += 1;
                }
            }
        }
    }

    free(row_1);
    free(row_2);

    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    change_image(height, width, image, 1);
    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    change_image(height, width, image, 2);
    return;
}
