#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// declaring functions
int check_alphabet(char char_in);
bool key_check(string key);
string cipher(string key, string plaintext);


// declaring globals
int alphabet_length = 26;


int main(int argc, string argv[])
{
    // ensures key is input
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // ensures key is correct length
    if (strlen(argv[1]) != alphabet_length)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    string key = argv[1];

    // checks characters entered into the key
    for (int i = 0; i < alphabet_length; i++)
    {
        if (check_alphabet(key[i]) == 0)
        {
            printf("Key must be alphabetic.\n");
            return 1;
        }
        else
        {
            key[i] = toupper(key[i]);
        }
    }

    // make sure there's no repeated letters
    if (key_check(key) == false)
    {
        printf("Only use each letter once in your key\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");

    // performs the cipher
    string ciphertext = cipher(key, plaintext);
    printf("ciphertext: %s\n", ciphertext);
}


// determines if a character is alphabetical and, if so, which case
int check_alphabet(char char_in)
{
    // for non-alphabetical
    if ((char_in > 122 || char_in < 65) || (char_in < 97 && char_in > 90))
    {
        return 0;
    }
    // for upper case
    else if (char_in < 97)
    {
        return 1;
    }
    // for lower case
    else
    {
        return 2;
    }
}


// checks to make sure only one of each letter
bool key_check(string key)
{
    for (int i = 0; i < alphabet_length; i++)
    {
        int current_char = key[i];

        for (int j = 0; j < (alphabet_length - i - 1); j++)
        {
            if (key[i + j + 1] == current_char)
            {
                return false;
            }
        }
    }

    return true;
}


// takes plaintext and uses the key to output ciphertext
string cipher(string key, string plaintext)
{
    for (int i = 0; i < strlen(plaintext); i++)
    {
        int char_type = check_alphabet(plaintext[i]);

        // upper case
        if (char_type == 1)
        {
            plaintext[i] = key[plaintext[i] - 65];
        }
        // lower case
        else if (char_type == 2)
        {
            plaintext[i] = tolower(key[plaintext[i] - 97]);
        }
    }

    return plaintext;
}
