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

    // Load sprite sheet texture
    if (!gModulatedTexture.LoadFromFile("../assets/colors.png")) {
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

    uint8_t r = 255, g = 255, b = 255;

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
                switch( e.key.keysym.sym ) {
                    //Increase red
                    case SDLK_q:
                        r += 32;
                        break;

                        //Increase green
                    case SDLK_w:
                        g += 32;
                        break;

                        //Increase blue
                    case SDLK_e:
                        b += 32;
                        break;

                        //Decrease red
                    case SDLK_a:
                        r -= 32;
                        break;

                        //Decrease green
                    case SDLK_s:
                        g -= 32;
                        break;

                        //Decrease blue
                    case SDLK_d:
                        b -= 32;
                        break;
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

        // Modulate and render texture
        gModulatedTexture.SetColor(r, g, b);
        gModulatedTexture.Render(0, 0);

        // Update screen
        SDL_RenderPresent(gRenderer);
    } // Main loop

    // Free resources and close SDL
    Close();

    return 0;
}


