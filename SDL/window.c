#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

/**
 * Used here
 * 0. SDL_Init
 * 1. SDL_Window
 * 2. SDL_Surface
 * 3. SDL_Event
 * 4. SDL_fillRect
 */
int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Error while SDL Init : %s\n", SDL_GetError());
    }

    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;
    bool doneDrawing = false;

    window = SDL_CreateWindow(
        "SDL_Demo 01", 0, 0, 450, 400, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        
    surface = SDL_GetWindowSurface(window);

    while(!doneDrawing)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                doneDrawing = true;
            }
        }
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, rand()%255, 255));
        SDL_UpdateWindowSurface(window);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}