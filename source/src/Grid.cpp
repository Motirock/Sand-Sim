#include <iostream>
#include <stdlib.h>

#include "Cell.h"
#include "Element.h"
#include "Grid.h"

Grid::Grid(int gridWidth, int gridHeight) : width(gridWidth), height(gridHeight) {
    time = 0;
    cellWidthPixels = 1600.0/width;
    cellHeightPixels = 900.0/height;
    cellGrid = new Cell*[width];
    for (int col = 0; col < width; col++)
        cellGrid[col] = new Cell[height];
    for (int col = 0; col < width; col++)
        for (int row = 0; row < height; row++)
            cellGrid[col][row] = Cell(col, row);
    std::cout << "width: " << width << " height: " << height << std::endl;
}

Grid::Grid() : width(100), height(100) {
    time = 0;
    cellWidthPixels = 1600.0/width;
    cellHeightPixels = 900.0/height;
    cellGrid = new Cell*[width];
    for (int col = 0; col < width; col++)
        cellGrid[col] = new Cell[height];
    for (int col = 0; col < width; col++)
        for (int row = 0; row < height; row++)
            cellGrid[col][row] = Cell(col, row);
}

Grid::~Grid() {
    free(cellGrid);
}

void Grid::update() {
    int startTimeMS = SDL_GetTicks();
    time++;
    for (int c = 0; c < width; c++) {
        for (int r = 0; r < height; r++) {
            cellGrid[c][r].element->hasBeenUpdated = false;
        }
    }
    if (time % 2 == 0) {
        for (int row = height-1; row >= 0; row--) {
            for (int col = 0; col < width; col++) {
                if (!cellGrid[col][row].element->hasBeenUpdated)
                    updateCell(col, row);
            }
        }
    }
    else {
        for (int row = height-1; row >= 0; row--) {
            for (int col = width-1; col >= 0; col--) {
                if (!cellGrid[col][row].element->hasBeenUpdated)
                    updateCell(col, row);
            }
        }
    }
    int endTimeMS = SDL_GetTicks()-startTimeMS;
    if (endTimeMS >= 10)
        std::cout << "Lag-spike! Frame time: " << (SDL_GetTicks()-startTimeMS) << " milliseconds" << std::endl;
}

void Grid::updateCell(int x, int y) {
    if (time % 8 < 4) {
        if (cellGrid[x][y].element->state == 2) {
            //Trying to move downward
            if (y+1 < height) {
                if (cellGrid[x][y+1].element->state == 0) {
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x-1 >= 0 && cellGrid[x-1][y+1].element->state == 0 && rand() % 100 < 90) {
                    cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 0 && rand() % 100 < 90) {
                    cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (cellGrid[x][y+1].element->state == 3 && rand() % 100 < 30) {
                    Element *tempElement = cellGrid[x][y+1].element;
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(tempElement);
                    return;
                }
                if (x-1 >= 0 && cellGrid[x-1][y+1].element->state == 3 && rand() % 100 < 10) {
                    Element *tempElement = cellGrid[x-1][y+1].element;
                    if (cellGrid[x-1][y].element->state == 0) {
                        cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                        cellGrid[x-1][y].setElement(tempElement);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                    }
                    else {
                        cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                        cellGrid[x][y].setElement(tempElement);
                    }
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 3 && rand() % 100 < 10) {
                    Element *tempElement = cellGrid[x+1][y+1].element;
                    if (cellGrid[x+1][y].element->state == 0) {
                        cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                        cellGrid[x+1][y].setElement(tempElement);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                    }
                    else {
                        cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                        cellGrid[x][y].setElement(tempElement);
                    };
                    return;
                }
            }
            cellGrid[x][y].element->hasBeenUpdated = true;
            return;
        }
        //Liquid
        if (cellGrid[x][y].element->state == 3) {
            //Trying to move downward
            if (y+1 < height) {
                if (cellGrid[x][y+1].element->state == 0) {
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x-1 >= 0 && cellGrid[x-1][y+1].element->state == 0) {
                    cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 0) {
                    cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
            }
            //Amount flowed max, depening on viscosisty
            int flowDistance = (rand() % 5)*cellGrid[x][y].element->viscosity+1;
            //Trying to flow right first
            if (rand() % 2 == 0 && rand() % 100 < cellGrid[x][y].element->viscosity*100) {
                for (int xOffset = 1; xOffset <= flowDistance; xOffset++) {
                    if (x+xOffset >= width || cellGrid[x+xOffset][y].element->state > 0) {
                        if (xOffset > 1) {
                            setElement(x+xOffset-1, y, cellGrid[x][y].element);
                            cellGrid[x][y].setElement(new Element(x, y, '-'));
                            return;
                        }
                        else {
                            cellGrid[x][y].element->hasBeenUpdated = true;
                        }
                        break;
                    }
                    if (xOffset == flowDistance) {
                        setElement(x+xOffset, y, cellGrid[x][y].element);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                        return;
                    }
                }
                for (int xOffset = -1; xOffset >= -flowDistance; xOffset--) {
                    if (x+xOffset < 0 || cellGrid[x+xOffset][y].element->state > 0) {
                        if (xOffset < -1) {
                            setElement(x+xOffset+1, y, cellGrid[x][y].element);
                            cellGrid[x][y].setElement(new Element(x, y, '-'));
                            return;
                        }
                        else {
                            cellGrid[x][y].element->hasBeenUpdated = true;
                        }
                        break;
                    }
                    if (xOffset == -flowDistance) {
                        setElement(x+xOffset, y, cellGrid[x][y].element);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                        return;
                    }
                }
            }
            else if (rand() % 100 < cellGrid[x][y].element->viscosity*100) {
                for (int xOffset = -1; xOffset >= -flowDistance; xOffset--) {
                    if (x+xOffset < 0 || cellGrid[x+xOffset][y].element->state > 0) {
                        if (xOffset < -1) {
                            setElement(x+xOffset+1, y, cellGrid[x][y].element);
                            cellGrid[x][y].setElement(new Element(x, y, '-'));
                            return;
                        }
                        break;
                    }
                    if (xOffset == -flowDistance) {
                        setElement(x+xOffset, y, cellGrid[x][y].element);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                        return;
                    }
                }
                for (int xOffset = 1; xOffset <= flowDistance; xOffset++) {
                    if (x+xOffset >= width || cellGrid[x+xOffset][y].element->state > 0) {
                        if (xOffset > 1) {
                            setElement(x+xOffset-1, y, cellGrid[x][y].element);
                            cellGrid[x][y].setElement(new Element(x, y, '-'));
                            return;
                        }
                        break;
                    }
                    if (xOffset == flowDistance) {
                        setElement(x+xOffset, y, cellGrid[x][y].element);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                        return;
                    }
                }
            }
            cellGrid[x][y].element->hasBeenUpdated = true;
            return;
        }
    }
    else {
        if (cellGrid[x][y].element->state == 2) {
            //Trying to move downward
            if (y+1 < height) {
                if (cellGrid[x][y+1].element->state == 0) {
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 0 && rand() % 100 < 90) {
                    cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x-1 >= 0 && cellGrid[x-1][y+1].element->state == 0 && rand() % 100 < 90) {
                    cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (cellGrid[x][y+1].element->state == 3 && rand() % 100 < 30) {
                    Element *tempElement = cellGrid[x][y+1].element;
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(tempElement);
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 3 && rand() % 100 < 10) {
                    Element *tempElement = cellGrid[x+1][y+1].element;
                    if (cellGrid[x+1][y].element->state == 0) {
                        cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                        cellGrid[x+1][y].setElement(tempElement);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                    }
                    else {
                        cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                        cellGrid[x][y].setElement(tempElement);
                    }
                    return;
                }
                if (x-1 >= 0 && cellGrid[x-1][y+1].element->state == 3 && rand() % 100 < 10) {
                    Element *tempElement = cellGrid[x-1][y+1].element;
                    if (cellGrid[x-1][y].element->state == 0) {
                        cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                        cellGrid[x-1][y].setElement(tempElement);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                    }
                    else {
                        cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                        cellGrid[x][y].setElement(tempElement);
                    }
                    return;
                }
            }
            cellGrid[x][y].element->hasBeenUpdated = true;
            return;
        }
        //Liquid
        if (cellGrid[x][y].element->state == 3) {
            //Trying to move downward
            if (y+1 < height) {
                if (cellGrid[x][y+1].element->state == 0) {
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 0) {
                    cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x-1 >= 0 && cellGrid[x-1][y+1].element->state == 0) {
                    cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
            }
            //Amount flowed max, depening on viscosisty
            int flowDistance = (rand() % 5)*cellGrid[x][y].element->viscosity+1;
            //Trying to flow right first
            if (rand() % 2 == 0 && rand() % 100 < cellGrid[x][y].element->viscosity*100) {
                for (int xOffset = 1; xOffset <= flowDistance; xOffset++) {
                    if (x+xOffset >= width || cellGrid[x+xOffset][y].element->state > 0) {
                        if (xOffset > 1) {
                            setElement(x+xOffset-1, y, cellGrid[x][y].element);
                            cellGrid[x][y].setElement(new Element(x, y, '-'));
                            return;
                        }
                        else {
                            cellGrid[x][y].element->hasBeenUpdated = true;
                        }
                        break;
                    }
                    if (xOffset == flowDistance) {
                        setElement(x+xOffset, y, cellGrid[x][y].element);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                        return;
                    }
                }
                for (int xOffset = -1; xOffset >= -flowDistance; xOffset--) {
                    if (x+xOffset < 0 || cellGrid[x+xOffset][y].element->state > 0) {
                        if (xOffset < -1) {
                            setElement(x+xOffset+1, y, cellGrid[x][y].element);
                            cellGrid[x][y].setElement(new Element(x, y, '-'));
                            return;
                        }
                        else {
                            cellGrid[x][y].element->hasBeenUpdated = true;
                        }
                        break;
                    }
                    if (xOffset == -flowDistance) {
                        setElement(x+xOffset, y, cellGrid[x][y].element);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                        return;
                    }
                }
            }
            else if (rand() % 100 < cellGrid[x][y].element->viscosity*100) {
                for (int xOffset = -1; xOffset >= -flowDistance; xOffset--) {
                    if (x+xOffset < 0 || cellGrid[x+xOffset][y].element->state > 0) {
                        if (xOffset < -1) {
                            setElement(x+xOffset+1, y, cellGrid[x][y].element);
                            cellGrid[x][y].setElement(new Element(x, y, '-'));
                            return;
                        }
                        break;
                    }
                    if (xOffset == -flowDistance) {
                        setElement(x+xOffset, y, cellGrid[x][y].element);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                        return;
                    }
                }
                for (int xOffset = 1; xOffset <= flowDistance; xOffset++) {
                    if (x+xOffset >= width || cellGrid[x+xOffset][y].element->state > 0) {
                        if (xOffset > 1) {
                            setElement(x+xOffset-1, y, cellGrid[x][y].element);
                            cellGrid[x][y].setElement(new Element(x, y, '-'));
                            return;
                        }
                        break;
                    }
                    if (xOffset == flowDistance) {
                        setElement(x+xOffset, y, cellGrid[x][y].element);
                        cellGrid[x][y].setElement(new Element(x, y, '-'));
                        return;
                    }
                }
            }
            cellGrid[x][y].element->hasBeenUpdated = true;
            return;
        }
    }
}

void Grid::render(SDL_Renderer *renderer) {
    SDL_Rect tempRect;
    tempRect.w = cellWidthPixels;
    tempRect.h = cellHeightPixels;
    Element *tempElement;
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            tempElement = cellGrid[col][row].element;
            tempRect.x = col*cellWidthPixels;
            tempRect.y = row*cellHeightPixels;
            SDL_SetRenderDrawColor(renderer, tempElement->colorRGB[0], tempElement->colorRGB[1], tempElement->colorRGB[2], 255);
            SDL_RenderFillRect(renderer, &tempRect);
        }
    }
}

Element* Grid::getElement(int x, int y) {
    return cellGrid[x][y].getElement();
}

void Grid::setElement(int x, int y, Element *element) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        element->hasBeenUpdated = true;
        cellGrid[x][y].setElement(element);
    }
}