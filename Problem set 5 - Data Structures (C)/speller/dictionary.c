// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;


typedef struct char_table
{
    int occupancy[28];
    struct char_table *character[27];
    char *strings[27];
}
char_table;


// declaring functions
// ------------------------------------------------
int char_position(char character);
int match_len(char *prev_word, char *current_word, char *next_word);
int match_two(char *word_one, int word_one_len, char *word_two, int word_two_len);
char_table *generate_table(void);
char *generate_string(char *word, int word_len, int match_length);
void table_unload(char_table *table_pointer);
// ------------------------------------------------

int word_count = 0;

// Choose number of buckets in hash table
const unsigned int N = 26;


// Hash table
char_table main_table;


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int word_len = strlen(word);
    char_table *current_table = &main_table;

    for (int i = 0; i < word_len; i++)
    {
        // returns false for unnoccupied paths
        if (current_table->occupancy[char_position(word[i])] == 0)
        {
            return false;
        }
        // if occupied it moves to the next table
        else if (current_table->occupancy[char_position(word[i])] == 1)
        {
            current_table = current_table->character[char_position(word[i])];
        }
        // if it's a string it checks to see if the string is correct
        else
        {
            // first check the lengths are the same
            int remaining = word_len - i - 1;
            if (strlen(current_table->strings[char_position(word[i])]) == remaining)
            {
                for (int j = 0; j < remaining; j++)
                {
                    if ((current_table->strings[char_position(word[i])])[j] != tolower(word[j + i + 1]))
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    // ensures that a sub-string word can be check using occupancy of the 28th position
    if (current_table->occupancy[27] == 1)
    {
        return true;
    }
    else
    {
        return false;
    }

    return true;
}



// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}


// hashes input word into tables
void improved_hash(char *current_word, int match_length)
{
    int current_char = -1;
    int word_len = strlen(current_word);
    char_table *current_table = &main_table;

    for (int i = 0; i < match_length; i++)
    {
        current_char = char_position(current_word[i]);

        // generates new table and set it as current table
        if (current_table->occupancy[current_char] == 0)
        {
            // pointer to new table put in character position and marked as occupied
            current_table->character[current_char] = generate_table();
            current_table->occupancy[current_char] = 1;

            current_table = current_table->character[current_char];
        }
        // if already occupied doesn't generate new table
        else
        {
            current_table = current_table->character[current_char];
        }
    }

    // if there are no more chars in the word it is marked as finished
    if (match_length == word_len)
    {
        current_table->occupancy[27] = 1;
    }
    // if word length is more than 1 longer than match length its stored as a string
    else if (word_len > match_length + 1)
    {
        current_char = char_position(current_word[match_length]);
        current_table->occupancy[current_char] = 2;

        current_table->strings[current_char] = generate_string(current_word, word_len, match_length);
    }
    // if word is 1 longer than match an extra table is created
    else
    {
        // creates extra table and sets occupancy
        current_char = char_position(current_word[match_length]);
        current_table->character[current_char] = generate_table();
        current_table->occupancy[current_char] = 1;

        // changes to new table and declares it's finished
        current_table = current_table->character[current_char];
        current_table->occupancy[27] = 1;
    }

    return;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // sets main table to unoccupied
    for (int i = 0; i < 28; i++)
    {
        main_table.occupancy[i] = 0;
    }

    // opens dictionary file
    FILE *dict_file = fopen(dictionary, "r");

    // returns false if file won't open
    if (dict_file == NULL)
    {
        return false;
    }

    // setting up all of the neccessary temporary variables
    char *temp_char = malloc(1);
    char *temp_word = NULL;
    char *prev_word = malloc(46);
    char *current_word = malloc(46);
    char *next_word = malloc(46);
    int letter_count = 0;
    int match_length = 0;
    prev_word[0] = '\0';
    current_word[0] = '\0';

    // reading and hashing the dictionary
    while (fread(temp_char, 1, 1, dict_file))
    {
        // when end of a word is seen the word is hashed
        if (temp_char[0] == '\n')
        {
            // string is marked as finished
            next_word[letter_count] = '\0';

            match_length = match_len(prev_word, current_word, next_word);

            if (word_count > 0)
            {
                improved_hash(current_word, match_length);
            }

            // swaps the string pointers around
            temp_word = prev_word;
            prev_word = current_word;
            current_word = next_word;
            next_word = temp_word;

            // word count increased and letter count reset
            word_count++;
            letter_count = 0;
        }
        // if the word doesn't end the file is still read to the same string
        else
        {
            next_word[letter_count] = temp_char[0];
            letter_count++;
        }
    }

    // hashes the last word
    improved_hash(current_word, match_len(prev_word, current_word, ""));

    // free temporary variables
    free(temp_char);
    free(prev_word);
    free(current_word);
    free(next_word);

    // closes dictionary file
    fclose(dict_file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (word_count > 0)
    {
        return word_count;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < 27; i++)
    {
        if (main_table.occupancy[i] == 1)
        {
            table_unload(main_table.character[i]);
        }
    }
    return true;
}



// previously declared functions...
// returns the position of an input char in the hash table
int char_position(char character)
{
    int position = 0;

    // converts all chars to uppercase and then offsets so it begins at zero
    if (character != 39)
    {
        position = toupper(character) - 65;
    }
    // sets the apostrophe char to the last position
    else
    {
        position = 26;
    }

    return position;
}


// returns the longest match length between surrounding words
int match_len(char *prev_word, char *current_word, char *next_word)
{
    // sets up initial variables
    int match_length = 0;
    int prev_len = strlen(prev_word);
    int current_len = strlen(current_word);
    int next_len = strlen(next_word);

    // finds match length between current and previous word
    int prev_match = 0;
    if (prev_len != 0)
    {
        prev_match = match_two(prev_word, prev_len, current_word, current_len);
    }

    // finds match length between current and next word
    int next_match = 0;
    if (next_len != 0)
    {
        next_match = match_two(current_word, current_len, next_word, next_len);
    }

    // sets match length to the longer match
    if (next_match > prev_match)
    {
        match_length = next_match;
    }
    else
    {
        match_length = prev_match;
    }

    return match_length;
}


// compares two strings and returns the maximum character they match to
int match_two(char *word_one, int word_one_len, char *word_two, int word_two_len)
{
    int match_length = 0;

    // cycles through words to see how many chars match and returns when there's a difference
    for (int i = 0; (i < word_one_len && i < word_two_len); i++)
    {
        if (word_one[i] == word_two[i])
        {
            match_length += 1;
        }
        else
        {
            return match_length;
        }
    }

    return match_length;
}


// returns the pointer of a newly generated table
char_table *generate_table(void)
{
    char_table *new_table = malloc(sizeof(char_table));

    // sets all to unoccupied
    for (int i = 0; i < 28; i++)
    {
        new_table->occupancy[i] = 0;
    }

    return new_table;
}


// generates a string containing the remaining chars
char *generate_string(char *word, int word_len, int match_length)
{
    char *new_string = malloc(sizeof(char) * (word_len - match_length));

    for (int i = match_length + 1; i < word_len; i++)
    {
        new_string[i - match_length - 1] = word[i];
    }

    new_string[word_len - match_length - 1] = '\0';

    return new_string;
}


//
void table_unload(char_table *table_pointer)
{
    // runs through occupancy table to see occupied cells of table
    for (int i = 0; i < 27; i++)
    {
        // when occupied with another table
        if (table_pointer->occupancy[i] == 1)
        {
            table_unload(table_pointer->character[i]);
        }
        // if occupied with a string
        else if (table_pointer->occupancy[i] == 2)
        {
            free(table_pointer->strings[i]);
        }
    }

    free(table_pointer);
}
