#include "Errors.h"
#include "SDL2/SDL.h"
#include <stdio.h>

void fatal_error(std::string error_string)
{
    printf("%s\n", error_string.c_str());
    printf("Enter any key to quit...\n");
    int temp;
    scanf("%d", &temp);
    SDL_Quit();
    exit(1);
}