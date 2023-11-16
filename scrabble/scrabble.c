#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};


int compute_score(string word);


int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Checks which score is higher and determines winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}


// returns the scrabble score of a given string
int compute_score(string word)
{
    // finds length of word and initialises the score
    int length = strlen(word);
    int score = 0;

    // cycles through all characters and scores them
    for (int i = 0; i < length; i++)
    {
        //only allows alphabet characters to be scored
        if ((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
        {
            int char_no = toupper(word[i]);
            int scrabble_value = POINTS[char_no - 65];
            score += scrabble_value;
        }
    }
    
    return score;
}
