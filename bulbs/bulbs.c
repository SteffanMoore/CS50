#include <cs50.h>
#include <stdio.h>
#include <string.h>


const int BITS_IN_BYTE = 8;


// declaring functions
void print_bulb(int bit);
string str_to_bin(char character);


int main(void)
{
    string message = get_string("Message: ");

    for (int i = 0; i < strlen(message); i++)
    {
        printf("%s\n", str_to_bin(message[i]));
    }
}


void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}


string str_to_bin(char character)
{
    int ASCII = character;
    string bin = "00000000";

    for (int j = BITS_IN_BYTE; j > 0; j--)
    {
        bin[j] = ASCII % 2;
        ASCII = ASCII / 2;
    }

    return bin;
}