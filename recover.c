#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Accept only one command-line argument
    if (argc != 2)
    {
        printf("Usage: /recover IMAGE\n");
        return 1;
    }

    // Open File
    //pointer keep tracks of the address // argv is the file // r is for the mode (open file for reading)
    FILE *input_file = fopen(argv[1], "r");


    // Check if can open the file
    if (input_file == NULL)
    {
        printf("IMAGE can't be opened\n");
        return 1;
    }

    // Declare variables

    // Group bytes into arrays of 512 bytes and store it in a buffer (unsigned for only positive values)
    unsigned char buffer[512];


    // Keep track of the number of images that we are generating
    int count_image = 0;


    // As we iterate through input_file and find JPEGs we need to write it into an output_file, this is a file pointer
    FILE *output_file = NULL; // NULL  because we are not writing anything yet


    // char filename[8] Alocate memory dinamically according to a size (8) space per char
    char *filename = malloc(8 * sizeof(char));


    // Read the file
    while (fread(buffer, sizeof(char), 512, input_file))
    {
        // Comb through each block until find the first 4 bytes indicate start of JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0Xf0) == 0xe0)
        {

            if (count_image > 0)
            {
                fclose(output_file);
            }

            // Write the JPEG filenames
            sprintf(filename, "%03i.jpg", count_image);

            // Open output_file for writing
            output_file = fopen(filename, "w");

            // Count number of images found
            count_image++;
        }
        // Check if output has been used for valid input
        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }

    }
    free(filename);
    fclose(output_file);
    fclose(input_file);

    return 0;
}