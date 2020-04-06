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

// Scene sprites(精灵)
SDL_Rect gSproteClips[4];
LTexture gSpriteSheetTexture;

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
    gSpriteSheetTexture.Free();

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

    // Load sprite sheet texture
    if (!gSpriteSheetTexture.LoadFromFile("../assets/dots.png")) {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    } else {
        // Set top left sprite
        gSproteClips[0].x = 0;
        gSproteClips[0].y = 0;
        gSproteClips[0].w = 100;
        gSproteClips[0].h = 100;

        // Set top right sprite
        gSproteClips[1].x = 100;
        gSproteClips[1].y = 0;
        gSproteClips[1].w = 100;
        gSproteClips[1].h = 100;

        // Set bottom left sprite
        gSproteClips[2].x = 0;
        gSproteClips[2].y = 100;
        gSproteClips[2].w = 100;
        gSproteClips[2].h = 100;

        // Set bottom right sprite
        gSproteClips[3].x = 100;
        gSproteClips[3].y = 100;
        gSproteClips[3].w = 100;
        gSproteClips[3].h = 100;
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
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // Render top left sprite
        gSpriteSheetTexture.Render(0, 0, &gSproteClips[0]);

        // Render top right sprite
        gSpriteSheetTexture.Render(SCREEN_WIDTH - gSproteClips[1].w, 0, &gSproteClips[1]);

        //Render bottom left sprite
        gSpriteSheetTexture.Render( 0, SCREEN_HEIGHT - gSproteClips[ 2 ].h, &gSproteClips[ 2 ] );

        //Render bottom right sprite
        gSpriteSheetTexture.Render( SCREEN_WIDTH - gSproteClips[ 3 ].w, SCREEN_HEIGHT - gSproteClips[ 3 ].h, &gSproteClips[ 3 ] );

        // Update screen
        SDL_RenderPresent(gRenderer);
    } // Main loop

    // Free resources and close SDL
    Close();

    return 0;
}


