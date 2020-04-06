//
// Created by plx on 2020/4/6.
//

#include <iostream>
#include "LTexture.h"

extern "C" {
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
}

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The  window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

LTexture gModulatedTexture;
LTexture gBackgroundTexture;

// Start up SDL and creates window
bool Init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL cound not init! SDL_Error: %s", SDL_GetError());
        success = false;
    } else {
        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
            success = false;
        } else {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                SDL_Log("Renderer could not be created! SDL Error: %s", SDL_GetError());
                success = false;
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initalize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    SDL_Log("SDL_image could not init! SDL_image Error: %s", IMG_GetError());
                    success = false;
                }/* else {
                    // Get window surface
                    gScreenSurface = SDL_GetWindowSurface(gWindow);
                }*/
            }
        }
    }

    return success;
}

// Frees media and shuts down SDL
void Close() {
    // Free loaded image
    gModulatedTexture.Free();
    gBackgroundTexture.Free();

    // Destory window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

// Loads media
bool LoadMedia() {
    bool success = true;

    // Load front alpha texture
    if (!gModulatedTexture.LoadFromFile("../assets/fadeout.png")) {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    } else {
        // Set standard alpha bleding
        gModulatedTexture.SetBlendMode(SDL_BLENDMODE_BLEND);
    }

    // Load background texture
    if (!gBackgroundTexture.LoadFromFile("../assets/fadein.png")) {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }

    return success;
}

int main() {
    if (!Init()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!LoadMedia()) {
        std::cout << "Failed to load media!" << std::endl;
        return -1;
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Modulation component
    uint8_t a = 255;

    // While application is running
    while (!quit) {
        // Handle events on queue
        // When the event queue is empty, SDL_PollEvent will return 0.
        //  So what this piece of code does is keep polling events off
        //  the event queue until it's empty.
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
                std::cout << "SDL_QUIT" << std::endl;
            } else if (e.type == SDL_KEYDOWN) {
                //Increase alpha on w
                if( e.key.keysym.sym == SDLK_w )
                {
                    //Cap if over 255
                    if( a + 32 > 255 )
                    {
                        a = 255;
                    }
                        //Increment otherwise
                    else
                    {
                        a += 32;
                    }
                }
                    //Decrease alpha on s
                else if( e.key.keysym.sym == SDLK_s )
                {
                    //Cap if below 0
                    if( a - 32 < 0 )
                    {
                        a = 0;
                    }
                        //Decrement otherwise
                    else
                    {
                        a -= 32;
                    }
                }
            }
        } // Event loop

        // SDL coordicate system
        // o------------------------> x
        // |
        // |
        // |
        // |
        // |
        // |
        // v
        // y

        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // Render background
        gBackgroundTexture.Render(0, 0);

        // Render front blended
        gModulatedTexture.SetAlpha(a);
        gModulatedTexture.Render(0, 0);

        // Update screen
        SDL_RenderPresent(gRenderer);
    } // Main loop

    // Free resources and close SDL
    Close();

    return 0;
}


