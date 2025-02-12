#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Error while SDL Init : %s\n", SDL_GetError());
    }

    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;
    SDL_Surface *bmpSurface[] = {NULL, NULL, NULL};
    bool doneDrawing = false;
    int currentIndex = 0;

    window = SDL_CreateWindow(
        "SDL_Demo 03", 0, 0, 1080, 720,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    bmpSurface[0] = SDL_LoadBMP("sample1.bmp");
    bmpSurface[1] = SDL_LoadBMP("sample2.bmp");
    bmpSurface[2] = SDL_LoadBMP("sample3.bmp");
    
    surface = SDL_GetWindowSurface(window);

    while(!doneDrawing)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                doneDrawing = true;
            }
            else if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_UP)
                {
                    currentIndex = (currentIndex + 1)%3;
                }
                if(event.key.keysym.sym == SDLK_DOWN)
                {
                    currentIndex = (currentIndex  + 2)%3;
                }
            }
        }
        SDL_Rect *destRect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
        destRect->x = 0;
        destRect->y = 0;
        destRect->w = 1080;
        destRect->h = 720;
        
        SDL_BlitScaled(bmpSurface[currentIndex], NULL, surface, destRect);
        SDL_UpdateWindowSurface(window);
    }
    SDL_DestroyWindow(window);
    SDL_FreeSurface(bmpSurface[0]);
    SDL_FreeSurface(bmpSurface[1]);
    SDL_FreeSurface(bmpSurface[2]);
    SDL_Quit();
    return 0;
}
