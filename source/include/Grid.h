#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "Cell.h"
#include "Element.h"

class Grid {
public:
    Grid(int width, int height);
    Grid();
    ~Grid();
    void update();
    void updateCell(int x, int y, int randomNumber);
    void render(SDL_Renderer *renderer);
    Element* getElement(int x, int y);
    void setElement(int x, int y, Element *element);
    void addToRectGroup(SDL_Rect *rect, int ID);
    void resetRectGroups();
    const int width, height, numThreads;
    float cellWidthPixels, cellHeightPixels;
    Cell** cellGrid;
    int time;
};