#include <cs50.h>
#include <stdio.h>

void print_char(char f, int n);

int main(void)
{
    // gets height input from user
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);


    //sets up printed characters
    char hash = '#';
    char space = ' ';


    // uses the print_char function to make the pyramid
    for (int i = 0; i < height; i++)
    {
        print_char(space, (height - i - 1));
        print_char(hash, i + 1);
        printf("  ");
        print_char(hash, i + 1);
        printf("\n");
    }
}


// prints characters multiple times
void print_char(char f, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", f);
    }
}