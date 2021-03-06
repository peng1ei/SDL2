//
// Created by plx on 2020/4/6.
//

#include "LTexture.h"

extern "C" {
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
}

extern SDL_Renderer *gRenderer;

LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    Free();
}

bool LTexture::LoadFromFile(const std::string &path) {
    // Get rid of preexisting texture
    Free();

    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    } else {
        // TODO Color key image
        // 设置透明
        SDL_SetColorKey(loadedSurface, SDL_TRUE,
                SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from srface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == NULL ) {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        } else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old laded srface
        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::Free() {
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = 0;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::Render(int x, int y, SDL_Rect *clip /*=NULL*/) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::GetWidth() {
    return mWidth;
}

int LTexture::GetHeight() {
    return mHeight;
}










