#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// each of our text files contains 1000 words
#define LISTSIZE 1000


// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0


// ANSI color codes for boxed in letters
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"


// user-defined function prototypes
string get_guess(int wordsize);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);


int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./wordle wordsize\n");
        return 1;
    }

    int wordsize = 0;

    // ensure argv[1] is either 5, 6, 7, or 8 and store that value in wordsize instead
    int arg_length = strlen(argv[1]);

    if (arg_length == 1)
    {
        if (argv[1][0] < 53 || argv[1][0] > 56)
        {
            printf("Error: wordsize must be either 5, 6, 7 or 8\n");
            return 1;
        }
        else
        {
            wordsize = (argv[1][0] - 48);
        }
    }


    // open correct file, each file has exactly LISTSIZE words
    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }

    // load word file into an array of size LISTSIZE
    char options[LISTSIZE][wordsize + 1];

    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }

    // pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;

    // print greeting, using ANSI color codes to demonstrate
    printf(GREEN"This is WORDLE50"RESET"\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);

    // main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // obtain user's guess
        string guess = get_guess(wordsize);

        // array to hold guess status, initially set to zero
        int status[wordsize];

        // set all elements of status array initially to 0, aka WRONG
        for (int j = 0; j < wordsize; j++)
        {
            status[j] = 0;
        }

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);

        printf("Guess %i: ", i + 1);

        // Print the guess
        print_word(guess, wordsize, status);

        // if they guessed it exactly right, set terminate loop
        if (score == EXACT * wordsize)
        {
            won = true;
            break;
        }
    }

    // Print the game's result
    if (won == true)
    {
        printf("You won!\n");
    }
    else
    {
        printf("The target word was %s.\n", choice);
    }

    // that's all folks!
    return 0;
}


// prompts user for guess string and checks it's correct length
string get_guess(int wordsize)
{
    string guess = "";

    do
    {
        guess = get_string("Input a %i-letter word: ", wordsize);
    }
    while (strlen(guess) != wordsize);

    return guess;
}


// checks guess against the actual word and returns a score
int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;

    // compare guess to choice and score points as appropriate, storing points in status
    for (int i = 0; i < wordsize; i++)
    {
        // for if the letter is in correct position
        if (guess[i] == choice[i])
        {
            status[i] = EXACT;
            score += EXACT;
        }
        // if letter isn't in exact position
        else
        {
            bool char_present = false;

            // iterates through choice word to see if letter is contained at all
            for (int j = 0; j < wordsize; j++)
            {
                // for if the letter is correct but not in the right place
                if (guess[i] == choice[j])
                {
                    char_present = true;
                }
            }

            if (char_present == true)
            {
                status[i] = CLOSE;
                score += CLOSE;
            }
            else
            {
                status[i] = WRONG;
            }
        }
    }

    return score;
}


// prints colour coded guess
void print_word(string guess, int wordsize, int status[])
{
    // iterates through length of the guess string
    for (int i = 0; i < wordsize; i++)
    {
        // status is used to determine colour
        if (status[i] == EXACT)
        {
            printf(GREEN"%c"RESET, guess[i]);
        }
        else if (status[i] == CLOSE)
        {
            printf(YELLOW"%c"RESET, guess[i]);
        }
        else
        {
            printf(RED"%c"RESET, guess[i]);
        }
    }

    printf("\n");
    return;
}
