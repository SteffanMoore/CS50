#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



// definitions
typedef uint8_t BYTE;
int block_length = 512;



// declaring functions
bool detect_new_JPEG(BYTE block[]);



// main function
int main(int argc, char *argv[])
{
    // ensures correct argument number
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // open the file for recovery
    FILE *forensic_image = fopen(argv[1], "r");

    if (forensic_image == NULL)
    {
        printf("File could not be opened. Please check correct file input.\n");
        return 1;
    }

    // allocate temporary memory
    BYTE temp_block[block_length - 1];

    // sets up the filename
    char file_name[8];

    // reads images into temp memory before outputting them
    int file = 0;
    int iteration = 0;
    bool start = false;

    while (fread(&temp_block, sizeof(BYTE), 512, forensic_image))
    {
        // creates new output file if new JPEG is detected
        if (detect_new_JPEG(temp_block) == true)
        {
            start = true;
            sprintf(file_name, "%03d.jpg", file);
            file++;
        }

        // opens and writes to output file
        if (start == true)
        {
            FILE *output = fopen(file_name, "a");
            fwrite(&temp_block, sizeof(BYTE), 512, output);
            fclose(output);
        }
    }
    fclose(forensic_image);
}



// detects the start of a new image
bool detect_new_JPEG(BYTE block[])
{
    // checks for correct starting bytes
    if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff)
    {
        return true;
    }

    return false;
}
