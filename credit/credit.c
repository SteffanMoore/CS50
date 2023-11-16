#include <cs50.h>
#include <stdio.h>


// prepares functions for later
string card_type(long start_number, long second_number, int luhn_number, int length);
int luhn_alg(int length, long digit);


// main function
int main(void)
{
    // initialises some variables
    long number = get_long("Number: ");
    int length = 0;
    int luhn_sum = 0;
    long first_digit = 0;
    long digit = 0;

    // negative numbers rejected
    if (number > -1)
    {
        // cycles through numbers where digits > 1
        if (number > 9)
        {
            do
            {
                digit = number % 10;
                number = number / 10;
                length++;
                luhn_sum += luhn_alg(length, digit);
            }
            while (number > 10);
        }

        length++;
        first_digit = number % 10;
        luhn_sum += luhn_alg(length, first_digit);
    }
    string validity = card_type(first_digit, digit, luhn_sum, length);
    printf("%s\n", validity);
}


// uses digit position to return digit value according to Luhn's algorithm
int luhn_alg(int length, long digit)
{
    int digit_value = 0;

    // runs through this for the digits which need doubling
    if (length % 2 == 0)
    {
        digit = digit * 2;

        // only runs through this if the doubled digit is > 10
        if (digit > 9)
        {
            digit_value += ((digit / 10) % 10);
        }

        digit_value += (digit % 10);
    }
    // runs through this is they don't need doubling
    else
    {
        digit_value += (digit);
    }

    return digit_value;
}


// tests which card type it could be
string card_type(long start_number, long second_number, int luhn_number, int length)
{
    string card;

    // rejects card if the luhn algorithm sum doesn't end in 0
    if (luhn_number % 10 != 0)
    {
        card = "INVALID";
    }
    else
    {
        // if luhn algorithm works, runs through card conditions to check
        if (start_number == 4 && (length == 16 || length == 13))
        {
            card = "VISA";
        }
        else if (start_number == 5 && second_number < 6 && length == 16)
        {
            card = "MASTERCARD";
        }
        else if (start_number == 3 && (second_number == 7 || second_number == 4) && length == 15)
        {
            card = "AMEX";
        }
        else
        {
            card = "INVALID";
        }
    }

    return card;
}