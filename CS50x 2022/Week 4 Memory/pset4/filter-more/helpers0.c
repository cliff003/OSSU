#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average of color in each pixel
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // Convert all color to average
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Set temp value
    RGBTRIPLE temp;

    // Stop reflecting at half of the width
    int half = round(width / 2.0);

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            // Swap pixels
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }

}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Set original value
    RGBTRIPLE original[height][width];

    // Save the original value for all pixels
    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            original[a][b] = image[a][b];
        }
    }

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Set calculation variables
            int sum_blue = 0, sum_green = 0, sum_red = 0;
            double count = 0.0;

            // Loop through adjacent pixels
            for (int m = i - 1; m <= i + 1; m++)
            {
                for (int n = j - 1; n <= j + 1; n++)
                {

                    // Make sure values within the boundary
                    if (m >= 0 && m < height && n >= 0 && n < width)
                    {
                        sum_blue += original[m][n].rgbtBlue;
                        sum_green += original[m][n].rgbtGreen;
                        sum_red += original[m][n].rgbtRed;
                        count += 1.0;
                    }
                }
            }

            // Calculate the average number
            image[i][j].rgbtBlue = round(sum_blue / count);
            image[i][j].rgbtGreen = round(sum_green / count);
            image[i][j].rgbtRed = round(sum_red / count);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Set original value
    RGBTRIPLE original[height][width];

    // Save the original value for all pixels
    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            original[a][b] = image[a][b];
        }
    }

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Set calculation variables
            long blue_Gx = 0, blue_Gy = 0, green_Gx = 0, green_Gy = 0, red_Gx = 0, red_Gy = 0;
            int Gx_coef = 0, Gy_coef = 0;

            // Loop through adjacent pixels
            for (int m = i - 1; m <= i + 1; m++)
            {
                for (int n = j - 1; n <= j + 1; n++)
                {

                    // Calculate Gx, Gy
                    if (m >= 0 && m < height && n >= 0 && n < width)
                    {

                        // Set coeficient for different situation
                        if (m == i - 1 && n == j - 1)
                        {
                            Gx_coef = -1;
                            Gy_coef = -1;
                        }
                        else if (m == i - 1 && n == j)
                        {
                            Gx_coef = 0;
                            Gy_coef = -2;
                        }
                        else if (m == i - 1 && n == j + 1)
                        {
                            Gx_coef = 1;
                            Gy_coef = -1;
                        }
                        else if (m == i && n == j - 1)
                        {
                            Gx_coef = -2;
                            Gy_coef = 0;
                        }
                        else if (m == i && n == j + 1)
                        {
                            Gx_coef = 2;
                            Gy_coef = 0;
                        }
                        else if (m == i + 1 && n == j - 1)
                        {
                            Gx_coef = -1;
                            Gy_coef = 1;
                        }
                        else if (m == i + 1 && n == j)
                        {
                            Gx_coef = 0;
                            Gy_coef = 2;
                        }
                        else if (m == i + 1 && n == j + 1)
                        {
                            Gx_coef = 1;
                            Gy_coef = 1;
                        }
                        else
                        {
                            Gx_coef = 0;
                            Gy_coef = 0;
                        }

                        // Get Gx, Gy for all three colors
                        blue_Gx += original[m][n].rgbtBlue * Gx_coef;
                        blue_Gy += original[m][n].rgbtBlue * Gy_coef;
                        green_Gx += original[m][n].rgbtGreen * Gx_coef;
                        green_Gy += original[m][n].rgbtGreen * Gy_coef;
                        red_Gx += original[m][n].rgbtRed * Gx_coef;
                        red_Gy += original[m][n].rgbtRed * Gy_coef;
                    }
                }
            }

            // Calculate the gradient magnitude G
            int blue_G = round(sqrt(blue_Gx * blue_Gx + blue_Gy * blue_Gy));
            int green_G = round(sqrt(green_Gx * green_Gx + green_Gy * green_Gy));
            int red_G = round(sqrt(red_Gx * red_Gx + red_Gy * red_Gy));

            // Cap the G at 255
            blue_G > 255 ? blue_G = 255 : blue_G;
            green_G > 255 ? green_G = 255 : green_G;
            red_G > 255 ? red_G = 255 : red_G;

            // Change pixel values
            image[i][j].rgbtBlue = blue_G;
            image[i][j].rgbtGreen = green_G;
            image[i][j].rgbtRed = red_G;
        }
    }
}