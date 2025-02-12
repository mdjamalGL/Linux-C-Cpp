#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

/**
 * Used Here :
 * 1. SDL_LoadBMP
 * 2. SDL_SetWindowSize : any surface that has been extracted
 *      from a window won't be resized if the the above is called after
 *      the GetWindowSurface
 * 3. SDL_BlitSurface : 
 *      Blitting means copying the pixel data from one memory section to other, 
 *      in CPU rendering, the Surface is used to copy the BMP loaded Surface to
 *      the window surface we use this function.
 * param : SDL_Rect has (x,y,w,h), only x and y is used to denoted from where the data
 *      has to be copied to or from. Thus no resize can be used
 *      We can use w,h with the alternate function SDL_BlitScaled.
 * 
 */
int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Error while SDL Init : %s\n", SDL_GetError());
    }

    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;
    SDL_Surface *bmpSurface = NULL;
    bool doneDrawing = false;

    window = SDL_CreateWindow(
        "SDL_Demo 02", 0, 0, 450, 400, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        
    
    bmpSurface = SDL_LoadBMP("sample.bmp");

    SDL_SetWindowSize(window, bmpSurface->w, bmpSurface->h);
    surface = SDL_GetWindowSurface(window);
    

    if(bmpSurface == NULL)
    {
        printf("Could not load BMP image\n");
        exit(1);
    }

    while(!doneDrawing)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                doneDrawing = true;
            }
        }

        SDL_BlitSurface(bmpSurface, NULL, surface, NULL);
        SDL_UpdateWindowSurface(window);
    }
    SDL_DestroyWindow(window);
    SDL_FreeSurface(bmpSurface);
    SDL_Quit();
    return 0;
}