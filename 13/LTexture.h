//
// Created by plx on 2020/4/6.
//

#ifndef IMOOC_LTEXTURE_H
#define IMOOC_LTEXTURE_H
#include <string>

extern "C" {
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
}

class SDL_Texture;
class SDL_Rect;

// Texture wrapper class
class LTexture {
public:
    // Initializes variables
    LTexture();
    ~LTexture();

    // Loads image at specified path
    bool LoadFromFile(const std::string &path);

    void Free();

    // Renders texture at given point
    void Render(int x, int y, SDL_Rect *clip = NULL);

    // Set color modulation
    void SetColor(uint8_t red, uint8_t green, uint8_t blue);

    // Set blending
    void SetBlendMode(SDL_BlendMode blending);
    // Set alpha modulation
    void SetAlpha(uint8_t alpha);

    int GetWidth();
    int GetHeight();

private:
    // The actual hardware texture
    SDL_Texture *mTexture;

    // Image dimensions
    int mWidth;
    int mHeight;
};

#endif //IMOOC_LTEXTURE_H
