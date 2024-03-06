#include "helpers.h"
#include <stdio.h>
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through pixel rows
    for (int i = 0; i < height; i++)
    {
        // Loop through pixels columns
        for (int j = 0; j < width; j ++)
        {
            // Compute average and apply grayscale filter
            BYTE avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loop through pixel rows
    for (int i = 0; i < height; i++)
    {
        // Loop through pixels columns
        for (int j = 0, k = width - 1; j < width && k >= 0 ; j++, k--)
        {
            // Reflect image using copy
            image[i][j] = copy[i][k];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loop through pixel rows
    for (int i = 0; i < height; i++)
    {
        // Loop through pixels columns
        for (int j = 0; j < width; j++)
        {
            float sum_red, sum_green, sum_blue;
            int count;

            sum_red = sum_green = sum_blue = count = 0;

            // Check if pixel is at edge/corner of image
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k >= height)
                    {
                        continue;
                    }

                    if (j + l < 0 || j + l >= width)
                    {
                        continue;
                    }

                    // Compute sum of each pixels and its surrounding pixels for each color values
                    sum_red += copy[i + k][j + l].rgbtRed;
                    sum_blue += copy[i + k][j + l].rgbtBlue;
                    sum_green += copy[i + k][j + l].rgbtGreen;
                    count++;
                }
            }

            // Get average and blur image
            image[i][j].rgbtRed = round(sum_red / count);
            image[i][j].rgbtGreen = round(sum_green / count);
            image[i][j].rgbtBlue = round(sum_blue / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Initialise Sobel arrays
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Loop through pixel rows
    for (int i = 0; i < height; i++)
    {
        // Loop through pixels columns
        for (int j = 0; j < width; j++)
        {
            float Gx_red, Gx_green, Gx_blue;
            float Gy_red, Gy_green, Gy_blue;

            Gx_red = Gx_blue = Gx_green = Gy_red = Gy_blue = Gy_green = 0;

            // Check if pixel is at edge/corner of image
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k >= height)
                    {
                        continue;
                    }

                    if (j + l < 0 || j + l >= width)
                    {
                        continue;
                    }

                    // Compute Gx for each pixels for each color values
                    Gx_red += copy[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                    Gy_red += copy[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];

                    Gx_green += copy[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                    Gy_green += copy[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];

                    Gx_blue += copy[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];
                    Gy_blue += copy[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                }
            }

            // Compute the square root of Gx^2 + Gy^2 for color values
            int red = round(sqrt(Gx_red * Gx_red + Gy_red * Gy_red));
            int green = round(sqrt(Gx_green * Gx_green + Gy_green * Gy_green));
            int blue = round(sqrt(Gx_blue * Gx_blue + Gy_blue * Gy_blue));

            // Cap at 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }

            // Apply edges filter
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}

