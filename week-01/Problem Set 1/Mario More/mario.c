#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // loop to get input on certain range (1 and 8; inclusive)
    do
    {
        height = get_int("How tall do you want the pyramid to be? ");
    }
    while (height < 1 || height > 8);


    // for column (height)
    for (int i = 0; i < height; i++)
    {

        // print something (spaces in this case) to align the pyramid on the right
        for (int k = height - 1; k > i; k--)
        {
            printf(" ");
        }
        // for lines
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        printf("  "); // spaces to separe the two pyramids

        // again print the pyramid, but without printing the dots (or spaces) to align it
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}