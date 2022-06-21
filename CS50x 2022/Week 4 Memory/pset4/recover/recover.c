#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

// Define data type
typedef uint8_t BYTE;

// Number of bytes in a block
const int BLOCK_SIZE = 512;

// Define filename size
const int FILENAME_SIZE = 8;

// Function prototype
bool check_header(uint8_t header[BLOCK_SIZE]);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open files
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Define variables
    uint8_t buffer[BLOCK_SIZE];
    int count = 0;
    char *filename = malloc(FILENAME_SIZE);

    while (fread(&buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {

        // Check if there is valid header
        while (check_header(buffer))
        {

            // Print filename and open output file
            sprintf(filename, "%03i.jpg", count);
            FILE *img = fopen(filename, "w");

            // Write header to the output file
            fwrite(&buffer, 1, BLOCK_SIZE, img);

            // Write remaining content to current file
            while (fread(&buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE && !check_header(buffer))
            {
                fwrite(&buffer, 1, BLOCK_SIZE, img);
            }

            // Close current file
            fclose(img);
            count++;
        }
    }

    // Close input file
    fclose(input);

    // Release memory
    free(filename);
}

bool check_header(uint8_t header[BLOCK_SIZE])
{
    // Check the header
    if (header[0] == 0xff && header[1] == 0xd8 && header[2] == 0xff && (header[3] & 0xf0) == 0xe0)
    {
        return true;
    }

    return false;
}