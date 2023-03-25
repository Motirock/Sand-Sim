#pragma once

#include <SDL2/SDL.h>

#include "Cell.h"
#include "Element.h"

class Grid {
public:
    Grid(int width, int height);
    Grid();
    ~Grid();
    void update();
    void render(SDL_Renderer *renderer);
    Element* getElement(int x, int y);
    void setElement(int x, int y, Element *element);
    const int width, height;
    Cell** cellGrid;
};