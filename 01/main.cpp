//
// Created by plx on 2020/4/6.
//

#include <iostream>

extern "C" {
#include <SDL2/SDL.h>
}

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main() {
    // The window we'll be rendering to
    SDL_Window* window = NULL;

    // The surface contained by the window
    // A SDL surface is just a 2D image. A 2D image can be loaded
    // from a file or it can be the image inside of a window.
    SDL_Surface* screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO/*SDL_INIT_EVERYTHING*/) < 0) {
        SDL_Log("SDL init failed! SDL_Error: %s", SDL_GetError());
        exit(1);
    }

    // Create window
    window = SDL_CreateWindow("SDL2 Demo",
                              //SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // we don't care where it is created
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!window) {
        SDL_Log("Failed to create window! SDL_Error: %s", SDL_GetError());
        exit(1);
    }

    //Get window surface
    screenSurface = SDL_GetWindowSurface( window );

    //Fill the surface white
    SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

    //Update the surface
    SDL_UpdateWindowSurface( window );

    //Wait two seconds
    SDL_Delay(2000);


    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


