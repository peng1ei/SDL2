//
// Created by plx on 2020/4/6.
//

#ifndef IMOOC_LTEXTURE_H
#define IMOOC_LTEXTURE_H
#include <string>

class SDL_Texture;

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
    void Render(int x, int y);

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
