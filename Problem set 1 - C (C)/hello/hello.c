#include <stdio.h>
#include <cs50.h>

// Printing name input
int main(void)
{
    string name = get_string("Enter your name: ");
    printf("hello, %s\n", name);
}
