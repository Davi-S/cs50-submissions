#include "helpers.h"
#include <math.h>

int max_255(int num)
{
    if (num > 255)
    {
        return 255;
    }
    else
    {
        return num;
    }
}

void swap(RGBTRIPLE *x, RGBTRIPLE *y)
{
    RGBTRIPLE temp = *x;
    *x = *y;
    *y = temp;
    return;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // avarage of the pixel RGB
            int avarage = round((image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3.0);

            image[h][w].rgbtBlue = avarage;
            image[h][w].rgbtGreen = avarage;
            image[h][w].rgbtRed = avarage;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++) // loop until the half of the image
        {
            swap(&image[h][w], &image[h][width - w - 1]); // swap pixels
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy of the original image
    RGBTRIPLE copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }

    // loop through all pixel of the image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int avarage_rgb[3] = {0}; // RED GREEN BLUE respectively
            float divisor = 0;

            // the pixel blur box occours like this:
            // ("h" is the row and "w" is the column of the pixel to blur)
            // image[h - 1][w - 1]    image[h - 1][w]    image[h - 1][w + 1]
            // image[h]    [w - 1]    image[h]    [w]    image[h]    [w + 1]
            // image[h + 1][w - 1]    image[h + 1][w]    image[h + 1][w + 1]

            // transforming this to arrays, we have:
            int hgt[3][3] = {{h - 1, h - 1, h - 1}, {h, h, h}, {h + 1, h + 1, h + 1}};
            int wdt[3][3] = {{w - 1, w, w + 1}, {w - 1, w, w + 1}, {w - 1, w, w + 1}};

            // loop to go through all pixels coordenates on the blur box
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    // if the hgt or wdt coordenates are negative or greater than height or width
                    // this means that the pixel it is outside the image
                    if ((hgt[x][y] >= 0 && wdt[x][y] >= 0) && (hgt[x][y] < height && wdt[x][y] < width))
                    {
                        // all this block means one pixel
                        // colect the RGB from this pixel
                        avarage_rgb[0] += copy[hgt[x][y]][wdt[x][y]].rgbtRed; // getting values from the copy image
                        avarage_rgb[1] += copy[hgt[x][y]][wdt[x][y]].rgbtGreen;
                        avarage_rgb[2] += copy[hgt[x][y]][wdt[x][y]].rgbtBlue;
                        divisor ++; // increase divisor by 1 on each pixel sucessfuly analized
                    }
                }
            }

            // get the actual avarage dividing the sum of the RGB by the number of pixels analized (divisor)
            avarage_rgb[0] = round(avarage_rgb[0] / divisor);
            avarage_rgb[1] = round(avarage_rgb[1] / divisor);
            avarage_rgb[2] = round(avarage_rgb[2] / divisor);

            // change the original image pixel with the avarage of the blur box
            image[h][w].rgbtRed = avarage_rgb[0];
            image[h][w].rgbtGreen = avarage_rgb[1];
            image[h][w].rgbtBlue = avarage_rgb[2];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy of the original image
    RGBTRIPLE copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }

    // loop through all pixel of the image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // RED GREEN BLUE respectively
            float sum_gx[3] = {0};
            float sum_gy[3] = {0};
            int final_kernel[3] = {0};

            // 3x3 box around the actual pixel
            int hgt[3][3] = {{h - 1, h - 1, h - 1}, {h, h, h}, {h + 1, h + 1, h + 1}};
            int wdt[3][3] = {{w - 1, w, w + 1}, {w - 1, w, w + 1}, {w - 1, w, w + 1}};

            // the number to multiply the pixels
            int kernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

            // loop through the 3x3 box around the pixel
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    // if the hgt or wdt coordenates are negative or greater than height or width
                    // this means that the pixel it is outside the image, and, its values are considerated all 0
                    // what will not afect the final kernel
                    if ((hgt[x][y] >= 0 && wdt[x][y] >= 0) && (hgt[x][y] < height && wdt[x][y] < width)) // only get valid pixels
                    {
                        sum_gx[0] += copy[hgt[x][y]][wdt[x][y]].rgbtRed * kernel[x][y];
                        sum_gx[1] += copy[hgt[x][y]][wdt[x][y]].rgbtGreen * kernel[x][y];
                        sum_gx[2] += copy[hgt[x][y]][wdt[x][y]].rgbtBlue * kernel[x][y];

                        // Gy kernel has its values rotated on the Cartesian plane
                        // to do this, switch x and y values on the kernel array
                        sum_gy[0] += copy[hgt[x][y]][wdt[x][y]].rgbtRed * kernel[y][x];
                        sum_gy[1] += copy[hgt[x][y]][wdt[x][y]].rgbtGreen * kernel[y][x];
                        sum_gy[2] += copy[hgt[x][y]][wdt[x][y]].rgbtBlue * kernel[y][x];
                    }
                }
            }
            // make the sum a positive number: fabsf()
            // square the sum: pow()
            // add the gx and gy
            // square root it: sqrt()
            // round to nearest integer: round()
            // truncate it if necessary: max_255()
            final_kernel[0] = max_255(round(sqrt(pow(fabsf(sum_gx[0]), 2) + pow(fabsf(sum_gy[0]), 2))));
            final_kernel[1] = max_255(round(sqrt(pow(fabsf(sum_gx[1]), 2) + pow(fabsf(sum_gy[1]), 2))));
            final_kernel[2] = max_255(round(sqrt(pow(fabsf(sum_gx[2]), 2) + pow(fabsf(sum_gy[2]), 2))));


            // change the original image pixel
            image[h][w].rgbtRed = final_kernel[0];
            image[h][w].rgbtGreen = final_kernel[1];
            image[h][w].rgbtBlue = final_kernel[2];
        }
    }
    return;
}

//     image[0][0].rgbtBlue = 0;
//     image[0][0].rgbtGreen = 10;
//     image[0][0].rgbtRed = 25;

//     image[0][1].rgbtBlue = 0;
//     image[0][1].rgbtGreen = 10;
//     image[0][1].rgbtRed = 30;

//     image[0][2].rgbtBlue = 40;
//     image[0][2].rgbtGreen = 60;
//     image[0][2].rgbtRed = 80;

//     image[1][0].rgbtBlue = 20;
//     image[1][0].rgbtGreen = 30;
//     image[1][0].rgbtRed = 90;

//     image[1][1].rgbtBlue = 30;
//     image[1][1].rgbtGreen = 40;
//     image[1][1].rgbtRed = 100;

//     image[1][2].rgbtBlue = 80;
//     image[1][2].rgbtGreen = 70;
//     image[1][2].rgbtRed = 90;

//     image[2][0].rgbtBlue = 20;
//     image[2][0].rgbtGreen = 20;
//     image[2][0].rgbtRed = 40;

//     image[2][1].rgbtBlue = 30;
//     image[2][1].rgbtGreen = 10;
//     image[2][1].rgbtRed = 30;

//     image[2][2].rgbtBlue = 50;
//     image[2][2].rgbtGreen = 40;
//     image[2][2].rgbtRed = 10;
