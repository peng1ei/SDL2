//
// Created by plx on 2020/4/6.
//

#include <iostream>

extern "C" {
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
}

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL,
};

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

// The images that correspond to a keypress
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

// Current displayed image
SDL_Surface *gCurrentSurface = NULL;

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
            // Initalize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                SDL_Log("SDL_image could not init! SDL_image Error: %s", IMG_GetError());
                success = false;
            } else {
                // Get window surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }

    return success;
}

// Loads media
bool LoadMedia() {
    bool success = true;

    // Load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = SDL_LoadBMP("../assets/press.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        SDL_Log("Failed to load default image!");
        success = false;
    }

    // Load up surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = SDL_LoadBMP("../assets/up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        SDL_Log("Failed to load up image!");
        success = false;
    }

    // Load down surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = SDL_LoadBMP("../assets/down.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        SDL_Log("Failed to load down image!");
        success = false;
    }

    // Load left surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = SDL_LoadBMP("../assets/left.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        SDL_Log("Failed to load left image!");
        success = false;
    }

    // Load right surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = SDL_LoadBMP("../assets/right.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        SDL_Log("Failed to load right image!");
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

// Loads individual image
SDL_Surface* LoadSurface(const std::string &path) {
    // The final optimized image
    SDL_Surface *optimizedSurface = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        SDL_Log("Unable to load image %s! IMG_Error: %s", path.c_str(),
                IMG_GetError());
    } else {
        // Convert surface to screen format
        // It's important to note that SDL_ConvertSurface
        // returns a copy of the original in a new format.
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL) {
            SDL_Log( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
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

    // Set default current surface
    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
    //gCurrentSurface = LoadSurface("../assets/loaded.png");

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
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                        break;
                    case SDLK_DOWN:
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                        break;
                    case SDLK_LEFT:
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                        break;
                    case SDLK_RIGHT:
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                        break;
                    default:
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                        break;
                }
            }
        } // Event loop

        // Apply the image
        // What blitting does is take a source surface and
        // stamps a copy of it onto the destination surface.
        // When you make draw calls like SDL_BlitSurface,
        // you render to the back buffer.
        //SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

        // Apply the image stretched
        SDL_Rect stretchRect;
        stretchRect.x = 100;
        stretchRect.y = 100;
        stretchRect.w = SCREEN_WIDTH-200;
        stretchRect.h = SCREEN_HEIGHT-200;
        SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);

        // Update the surface
        // What you see on the screen is the front buffer.
        // 类似于交换前后缓冲区（双缓冲区）
        SDL_UpdateWindowSurface(gWindow);
    } // Main loop

    // Free resources and close SDL
    Close();

    return 0;
}


