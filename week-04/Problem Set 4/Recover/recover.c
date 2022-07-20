#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int BLOCK_SIZE = 512;
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // get valid arguments
    if (argc > 2 || argc < 2)
    {
        printf("usage: ./recover FORENSIC_IMAGE");
        return 1;
    }

    FILE *recover = fopen(argv[1], "r"); // open file
    // TODO check if its a valid file or if it was opened without errors

    BYTE buffer[512] = {0};
    int count = 0;
    FILE *image; // create image reference
    while (fread(buffer, 1, BLOCK_SIZE, recover) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0)) // start of a jpeg
        {
            if (count != 0) // not the first jpeg -> shold close previous one
            {
                fclose(image);
            }

            // create the file with the proper name and increase count
            char filename[8];
            sprintf(filename, "%03i.jpg", count);
            image = fopen(filename, "w");
            count++;

            // write on the file the actual FAT
            fwrite(buffer, 1, BLOCK_SIZE, image);
        }

        else if (count != 0) // the first jpeg was aready found and the actual FAT is not the start of a new jpeg
        {
            fwrite(buffer, 1, BLOCK_SIZE, image); // write FAT on the file
        }
    }

    // close files
    fclose(image);
    fclose(recover);
    return 0;
}
