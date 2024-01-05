#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input output\n");
        return 1;
    }

    // Open input file for reading
    FILE *input_file = fopen(argv[1], "r");

    if (input_file == NULL)
    {
        printf("Input file not found\n");
        return 1;
    }

    // Read header
    WAVHEADER file_header;
    fread(&file_header, sizeof(BYTE), 44, input_file);

    // Use check_format to ensure WAV format
    if (check_format(file_header) == 1)
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *output_file = fopen(argv[2], "w");

    // Write header to file
    fwrite(&file_header, sizeof(BYTE), 44, output_file);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(file_header);

    // Write reversed audio to file
    fseek(input_file, -block_size, SEEK_END);
    BYTE *temp_block = malloc(block_size * sizeof(BYTE));
    while (ftell(input_file) >= 44)
    {
        fread(&temp_block, sizeof(BYTE), block_size, input_file);
        fseek(input_file, -block_size * 2, SEEK_CUR);
        fwrite(&temp_block, sizeof(BYTE), block_size, output_file);
    }


    // close the files
    fclose(input_file);
    fclose(output_file);
}


// returns 0 if the header is in WAV format
int check_format(WAVHEADER header)
{
    char *wave = "WAVE";

    // cycles through format section to make sure it is "WAVE"
    for (int i = 0; i < 4; i++)
    {
        if (wave[i] != header.format[i])
        {
            return 1;
        }
    }

    return 0;
}


// returns number of bytes in a block
int get_block_size(WAVHEADER header)
{
    int bytes_per_sample = header.bitsPerSample / 8;
    int channels = header.numChannels;
    int block_size = channels * bytes_per_sample;

    return block_size;
}
