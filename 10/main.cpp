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

// Sceen textures
LTexture gFooTexture;
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
    gFooTexture.Free();
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

SDL_Texture* LoadTexture(const std::string &path) {
    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        SDL_Log( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    } else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            SDL_Log( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

// Loads media
bool LoadMedia() {
    bool success = true;

    // Load Foo' texture
    if (!gFooTexture.LoadFromFile("../assets/foo.png")) {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    // Load backround texture
    if (!gBackgroundTexture.LoadFromFile("../assets/background.png")) {
        printf( "Failed to load Foo' texture image!\n" );
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
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // Render background texture to screen
        gBackgroundTexture.Render(0, 0);

        // Render Foo' to the screen;
        gFooTexture.Render(240, 290);

        // Update screen
        SDL_RenderPresent(gRenderer);
    } // Main loop

    // Free resources and close SDL
    Close();

    return 0;
}


