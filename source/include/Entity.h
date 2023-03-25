#pragma once

#include "Game.h"

class Entity {
public:
    Entity(const char *textureSheet, SDL_Renderer *renderer, int x, int y);
    ~Entity();

    void update();
    void render();
private:
    int x;
    int y;
    SDL_Texture *texture;
    SDL_Rect sourceRect, destinationRect;
    SDL_Renderer* renderer;

};