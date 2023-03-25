#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager {
public:
    static SDL_Texture* loadTexture(const char *fileName, SDL_Renderer *renderer);
};