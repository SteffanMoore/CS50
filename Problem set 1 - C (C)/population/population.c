#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size (9 or greater)
    int population;

    do
    {
        population = get_int("Input initial population size: ");
    }
    while (population < 9);


    // Prompt for end size
    int end_population;

    do
    {
        end_population = get_int("Input the end population: ");
    }
    while (end_population < population);


    // Calculate number of years until we reach threshold
    int year = 0;
    int population_change;

    if (population < end_population)
    {
        do
        {
            population_change = population / 3 - population / 4;
            population += population_change;
            year++;
        }
        while (population < end_population);
    }
    

    // Print number of years
    printf("Years: %i\n", year);
}
