#include "TextureManager.h"

SDL_Texture* TextureManager::loadTexture(const char *fileName, SDL_Renderer *renderer) {
    SDL_Surface *tempSurface = IMG_Load(fileName);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return(texture);
}