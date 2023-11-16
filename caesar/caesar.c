#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// setting up the alphabet
string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


// declaring functions
int str_to_num(string str_in);
string cipher(string plaintext, int key);
char char_change(char character, int key);


int main(int argc, string argv[])
{
    // forces use of two arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // allows only numerical chars in the key
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (argv[1][i] < 48 || argv[1][i] > 57)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // gets the number of places to offset for the key
    int key = str_to_num(argv[1]);

    // retrieves and then encodes the cipher
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: %s\n", cipher(plaintext, key));
}


// converts string into integer
int str_to_num(string str_in)
{
    int offset = 0;
    int magnitude = strlen(str_in);

    // runs through string to turn into int
    for (int i = 0; i < magnitude; i++)
    {
        int new_digit = str_in[i] - 48;

        // makes sure digits have correct base 10 value
        for (int j = 0; j < magnitude - i - 1; j++)
        {
            new_digit = new_digit * 10;
        }

        offset += new_digit;
    }

    return offset;
}


// output ciphertext
string cipher(string plaintext, int key)
{
    for (int i = 0; i < strlen(plaintext); i++)
    {
        // upper case
        if (plaintext[i] < 91 && plaintext[i] > 64)
        {
            plaintext[i] = char_change(plaintext[i], key);
        }
        // lower case
        else if (plaintext[i] < 123 && plaintext[i] > 96)
        {
            plaintext[i] = toupper(plaintext[i]);
            plaintext[i] = char_change(plaintext[i], key);
            plaintext[i] = tolower(plaintext[i]);
        }
    }

    return plaintext;
}


// handles individual characters for cipher
char char_change(char character, int key)
{
    do
    {
        if ((character + key) > 90)
        {
            character -= 26;
        }
    }
    while ((character + key) > 90);

    character += key;

    return character;
}