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


// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // get original values
            int blue = image[h][w].rgbtBlue;
            int green = image[h][w].rgbtGreen;
            int red = image[h][w].rgbtRed;

            // apply the sepia algorithm
            // using max_255 to not overflow the pixel RGB limit
            image[h][w].rgbtRed = max_255(round(0.393 * red + 0.769 * green + 0.189 * blue));
            image[h][w].rgbtGreen = max_255(round(0.349 * red + 0.686 * green + 0.168 * blue));
            image[h][w].rgbtBlue = max_255(round(0.272 * red + 0.534 * green + 0.131 * blue));
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
                    // if the pixel is negative, this means that it is outside the image (upper and left corner)
                    // only get RGB from pixels inside the image
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
