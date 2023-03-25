#include "Entity.h"
#include "Game.h"
#include "TextureManager.h"

#include <iostream>

Entity::Entity(const char *textureSheet, SDL_Renderer *renderer, int x, int y) {
    this->renderer = renderer;
    this->x = x;
    this->y = y;
    texture = TextureManager::loadTexture(textureSheet, renderer);

    sourceRect.x = 0;
    sourceRect.y = 0;
    sourceRect.w = 32;
    sourceRect.h = 32;
}

void Entity::update() {
    destinationRect.x = x++;
    destinationRect.y = y;
    destinationRect.w = 100;
    destinationRect.h = 100;
}

void Entity::render() {
    SDL_RenderCopy(renderer, texture, &sourceRect, &destinationRect);
}