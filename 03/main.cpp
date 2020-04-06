//
// Created by plx on 2020/4/6.
//

#include <iostream>

extern "C" {
#include <SDL2/SDL.h>
}

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The  window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// The image we will load and show on the screen
//  An SDL surface is just an image data type that
//  contains the pixels of an image along with all
//  data needed to render it. SDL surfaces use software
//  rendering which means it uses the CPU to render.
SDL_Surface *gHelloWorld = NULL;

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
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

// Loads media
bool LoadMedia() {
    bool success = true;

    // Load splash image
    gHelloWorld = SDL_LoadBMP("../assets/hello_world.bmp");
    if (gHelloWorld == NULL) {
        SDL_Log("Unable to load image %s! SDL_Error: %s", "../assets/hello_world.bmp", SDL_GetError());
        success = false;
    }

    return success;
}

// Frees media and shuts down SDL
void Close() {
    // Deallocate surface
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    // Destory window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
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

        // Apply the image
        // What blitting does is take a source surface and
        // stamps a copy of it onto the destination surface.
        // When you make draw calls like SDL_BlitSurface,
        // you render to the back buffer.
        SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

        // Update the surface
        // What you see on the screen is the front buffer.
        // 类似于交换前后缓冲区（双缓冲区）
        SDL_UpdateWindowSurface(gWindow);
    } // Main loop

    // Free resources and close SDL
    Close();

    return 0;
}


