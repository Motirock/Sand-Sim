#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <thread>

#include "Cell.h"
#include "Element.h"
#include "ThreadPool.hpp"

class Grid {
public:
    Grid(int width, int height);
    Grid();
    ~Grid();
    void update();
    //void updateCell(int x, int y, int randomNumber);
    void render(SDL_Renderer *renderer);
    void updateCell(int x, int y, int randomNumber);
    Element* getElement(int x, int y);
    void setElement(int x, int y, Element *element);
    void addToRectGroup(SDL_Rect *rect, int ID);
    void resetRectGroups();
    const int width, height, numThreads, numTasks;
    float cellWidthPixels, cellHeightPixels;
    Cell** cellGrid;
    int time;
    bool running;
};