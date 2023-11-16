#include <cs50.h>
#include <stdio.h>
#include <math.h>

float index(float letter_average, float sentence_average);

int main(void)
{
    // gets text string
    char *text = get_string("Text: ");

    // gives starting conditions
    char temp_char = ' ';
    int char_pos = 0;

    // sets up incremental variables
    int letter_no = 0;
    int word_no = 0;
    int sentence_no = 0;

    // goes through all chars in input text to find letter/sentence/word numbers
    do
    {
        temp_char = text[char_pos];
        char_pos++;

        // checks for alphabetical character
        if ((temp_char < 123 && temp_char > 96) || (temp_char < 91 && temp_char > 64))
        {
            letter_no++;
        }
        // checks for full stop, exclaimation mark and question mark
        else if (temp_char == '.' || temp_char == '?' || temp_char == '!')
        {
            sentence_no++;
        }
        // checks for spaces
        else if (temp_char == ' ')
        {
            word_no++;
        }
    }
    while (temp_char != '\0');

    word_no++;

    // finds average letters/sentences per 100 words
    float letters_per_100 = ((float)letter_no / (float)word_no) * 100;
    float sentences_per_100 = ((float)sentence_no / (float)word_no) * 100;



    // calculates reading grade index
    float coleman_lieu_index = index(letters_per_100, sentences_per_100);
    int grade = (int)round(coleman_lieu_index);

    // handles higher/lower than possible reading grades and prints grade level
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

// returns the Coleman-Lieu index for given letter/sentence length averages
float index(float letter_average, float sentence_average)
{
    float index_value = 0.0588 * letter_average - 0.296 * sentence_average - d15.8;
    return index_value;
}