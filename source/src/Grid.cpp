#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>

#include "Cell.h"
#include "Element.h"
#include "Grid.h"
#include "Utils.hpp"

void updateRegion(Grid *grid, int ID) {
    int leftLimit = (int) (grid->width/grid->numThreads*ID);
    int rightLimit = (int) (grid->width/grid->numThreads*(ID+1));

    for (int c = leftLimit; c < rightLimit; c++) {
        for (int r = 0; r < grid->height; r++) {
            grid->cellGrid[c][r].element->hasBeenUpdated = false;
        }
    }

    if (grid->time % 12 < 6) {
        if (grid->time % 2 == 0) {
            for (int row = grid->height-1; row >= 0; row--) {
                for (int col = leftLimit; col < rightLimit; col++) {
                    if (!grid->cellGrid[col][row].element->hasBeenUpdated)
                        grid->updateCell(col, row, rand() % 101);
                }
            }
        }
        else {
            for (int col = leftLimit; col < rightLimit; col++) {
                for (int row = grid->height-1; row >= 0; row--) {
                    if (!grid->cellGrid[col][row].element->hasBeenUpdated)
                        grid->updateCell(col, row, rand() % 101);
                }
            }
        }
    }
    else {
        if (grid->time % 2 == 0) {
            for (int row = grid->height-1; row >= 0; row--) {
                for (int col = rightLimit-1; col >= leftLimit; col--) {
                    if (!grid->cellGrid[col][row].element->hasBeenUpdated)
                        grid->updateCell(col, row, rand() % 101);
                }
            }
        }
        else {
            for (int col = rightLimit-1; col >= leftLimit; col--) {
                for (int row = grid->height-1; row >= 0; row--) {
                    if (!grid->cellGrid[col][row].element->hasBeenUpdated)
                        grid->updateCell(col, row, rand() % 101);
                }
            }
        }
    }
}

Grid::Grid(int gridWidth, int gridHeight) : width(gridWidth), height(gridHeight), numThreads(5) {
    time = 0;
    cellWidthPixels = 1600.0/width;
    cellHeightPixels = 900.0/height;
    cellGrid = new Cell*[width];
    for (int col = 0; col < width; col++)
        cellGrid[col] = new Cell[height];
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            cellGrid[col][row] = Cell(col, row);
            if (cellGrid[col][row].element->colorID == 0)
                cellGrid[col][row] = Cell(col, row);
        }
    }
    std::cout << "width: " << width << " height: " << height << std::endl;
}

Grid::Grid() : width(100), height(100), numThreads(10) {
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

    std::thread threads[numThreads];
    for (int i  = 0; i < numThreads; i++) {
        threads[i] = std::thread(updateRegion, this, i);
    }
    for (int i  = 0; i < numThreads; i++) {
        threads[i].join();
    }

    int endTimeMS = SDL_GetTicks()-startTimeMS;

    /*if (endTimeMS >= 10)
        std::cout << "Lag-spike! Frame time: " << endTimeMS << " milliseconds" << std::endl;*/
    if (time % 10 == 0) {
        std::cout << "Update time: " << endTimeMS << " milliseconds" << std::endl;
    }
}

void Grid::updateCell(int x, int y, int randomNumber) {
    if (time % 8 < 4) {
        //Falling solid
        if (cellGrid[x][y].element->state == 2) {
            //Trying to move downward
            if (y+1 < height) {
                if (cellGrid[x][y+1].element->state == 0) {
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x > 0 && cellGrid[x-1][y+1].element->state == 0 && randomNumber < 90) {
                    cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 0 && randomNumber < 90) {
                    cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if ((cellGrid[x][y+1].element->state == 3 && randomNumber < 30) || cellGrid[x][y+1].element->state == 4) {
                    Element *tempElement = cellGrid[x][y+1].element;
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(tempElement);
                    return;
                }
                if (x > 0 && ((cellGrid[x-1][y+1].element->state == 3 && randomNumber < 10) || cellGrid[x-1][y+1].element->state == 4)) {
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
                if (x+1 < width && ((cellGrid[x+1][y+1].element->state == 3 && randomNumber < 10) || cellGrid[x+1][y+1].element->state == 4)) {
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
            //Checks for water
            if (cellGrid[x][y].element->type == 3) {
                //Looking for lava
                if (y+1 < height && cellGrid[x][y+1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x][y+1].setElement(new Element(x, y+1, 's'));
                    return;
                }
                if (y+1 < height && x > 0 && cellGrid[x-1][y+1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x-1][y+1].setElement(new Element(x-1, y+1, 's'));
                    return;
                }
                if (y+1 < height && x+1 < width && cellGrid[x+1][y+1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x+1][y+1].setElement(new Element(x+1, y+1, 's'));
                    return;
                }
                if (x > 0 && cellGrid[x-1][y].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x-1][y].setElement(new Element(x-1, y, 's'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x+1][y].setElement(new Element(x+1, y, 's'));
                    return;
                }
                if (y > 0 && cellGrid[x][y-1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, 's'));
                    cellGrid[x][y-1].setElement(new Element(x, y-1, '-'));
                    return;
                }
                if (y > 0 && x > 0 && cellGrid[x-1][y-1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, 's'));
                    cellGrid[x-1][y-1].setElement(new Element(x-1, y-1, '-'));
                    return;
                }
                if (y > 0 && x+1 < width && cellGrid[x+1][y-1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, 's'));
                    cellGrid[x+1][y-1].setElement(new Element(x+1, y-1, '-'));
                    return;
                }
            }

            //Trying to move downward
            if (y+1 < height) {
                if (cellGrid[x][y+1].element->state == 0) {
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x > 0 && cellGrid[x-1][y+1].element->state == 0) {
                    cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 0) {
                    cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (cellGrid[x][y+1].element->state == 4 && randomNumber < 90) {
                    Element *tempElement = cellGrid[x][y+1].element;
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(tempElement);
                    return;
                }
                if (x > 0 && cellGrid[x-1][y+1].element->state == 4 && randomNumber < 50) {
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
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 4 && randomNumber < 50) {
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
            
            //Amount flowed max, depening on viscosisty
            int flowDistance = (rand() % 5)*cellGrid[x][y].element->viscosity+1;
            //Trying to flow right first
            if (rand() % 2 == 0 && randomNumber < cellGrid[x][y].element->viscosity*100) {
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
            else if (randomNumber < cellGrid[x][y].element->viscosity*100) {
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
        
        //Gas
        if (cellGrid[x][y].element->state == 4) {
            //Trying to move upward
            if (y > 0) {
                if (cellGrid[x][y-1].element->state == 0) {
                    cellGrid[x][y-1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x > 0 && cellGrid[x-1][y-1].element->state == 0) {
                    cellGrid[x-1][y-1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y-1].element->state == 0) {
                    cellGrid[x+1][y-1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
            }
            //Amount flowed max, depening on viscosisty
            int flowDistance = (rand() % 5)*cellGrid[x][y].element->viscosity+1;
            //Trying to flow right first
            if (rand() % 2 == 0 && randomNumber < cellGrid[x][y].element->viscosity*100) {
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
            else if (randomNumber < cellGrid[x][y].element->viscosity*100) {
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
        //Falling solid
        if (cellGrid[x][y].element->state == 2) {
            //Trying to move downward
            if (y+1 < height) {
                if (cellGrid[x][y+1].element->state == 0) {
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 0 && randomNumber < 90) {
                    cellGrid[x+1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x > 0 && cellGrid[x-1][y+1].element->state == 0 && randomNumber < 90) {
                    cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if ((cellGrid[x][y+1].element->state == 3 && randomNumber < 30) || cellGrid[x][y+1].element->state == 4) {
                    Element *tempElement = cellGrid[x][y+1].element;
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(tempElement);
                    return;
                }
                if (x+1 < width && ((cellGrid[x+1][y+1].element->state == 3 && randomNumber < 10) || cellGrid[x+1][y+1].element->state == 4)) {
                    Element *tempElement = cellGrid[x+1][y+1].element;
                    if (cellGrid[x+1][y].element->state >= 0) {
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
                if (x > 0 && ((cellGrid[x-1][y+1].element->state == 3 && randomNumber < 10) || cellGrid[x-1][y+1].element->state == 4)) {
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
            //Checks for water
            if (cellGrid[x][y].element->type == 3) {
                //Looking for lava
                if (y+1 < height && cellGrid[x][y+1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x][y+1].setElement(new Element(x, y+1, 's'));
                    return;
                }
                if (y+1 < height && x+1 < width && cellGrid[x+1][y+1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x+1][y+1].setElement(new Element(x+1, y+1, 's'));
                    return;
                }
                if (y+1 < height && x > 0 && cellGrid[x-1][y+1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x-1][y+1].setElement(new Element(x-1, y+1, 's'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x+1][y].setElement(new Element(x+1, y, 's'));
                    return;
                }
                if (x > 0 && cellGrid[x-1][y].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    cellGrid[x-1][y].setElement(new Element(x-1, y, 's'));
                    return;
                }
                if (y > 0 && cellGrid[x][y-1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, 's'));
                    cellGrid[x][y-1].setElement(new Element(x, y-1, '-'));
                    return;
                }
                if (y > 0 && x+1 < width && cellGrid[x+1][y-1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, 's'));
                    cellGrid[x+1][y-1].setElement(new Element(x+1, y-1, '-'));
                    return;
                }
                if (y > 0 && x > 0 && cellGrid[x-1][y-1].element->type == 4) {
                    cellGrid[x][y].setElement(new Element(x, y, 's'));
                    cellGrid[x-1][y-1].setElement(new Element(x-1, y-1, '-'));
                    return;
                }
            }

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
                if (x > 0 && cellGrid[x-1][y+1].element->state == 0) {
                    cellGrid[x-1][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (cellGrid[x][y+1].element->state == 4 && randomNumber < 90) {
                    Element *tempElement = cellGrid[x][y+1].element;
                    cellGrid[x][y+1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(tempElement);
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y+1].element->state == 4 && randomNumber < 50) {
                    Element *tempElement = cellGrid[x+1][y+1].element;
                    if (cellGrid[x+1][y].element->state >= 0) {
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
                if (x > 0 && cellGrid[x-1][y+1].element->state == 4 && randomNumber < 50) {
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
            
            //Amount flowed max, depening on viscosisty
            int flowDistance = (rand() % 5)*cellGrid[x][y].element->viscosity+1;
            //Trying to flow right first
            if (rand() % 2 == 0 && randomNumber < cellGrid[x][y].element->viscosity*100) {
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
            else if (randomNumber < cellGrid[x][y].element->viscosity*100) {
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
        
        //Gas
        if (cellGrid[x][y].element->state == 4) {
            //Trying to move upward
            if (y > 0) {
                if (cellGrid[x][y-1].element->state == 0) {
                    cellGrid[x][y-1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x+1 < width && cellGrid[x+1][y-1].element->state == 0) {
                    cellGrid[x+1][y-1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
                if (x > 0 && cellGrid[x-1][y-1].element->state == 0) {
                    cellGrid[x-1][y-1].setElement(cellGrid[x][y].element);
                    cellGrid[x][y].setElement(new Element(x, y, '-'));
                    return;
                }
            }
            //Amount flowed max, depening on viscosisty
            int flowDistance = (rand() % 5)*cellGrid[x][y].element->viscosity+1;
            //Trying to flow left first
            if (rand() % 2 == 0 && randomNumber < cellGrid[x][y].element->viscosity*100) {
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
            else if (randomNumber < cellGrid[x][y].element->viscosity*100) {
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
            cellGrid[x][y].element->hasBeenUpdated = true;
            return;
        }
    }
}

void Grid::render(SDL_Renderer *renderer) {
    int startTimeMS = SDL_GetTicks();

    std::vector<SDL_Rect> rects101;
    std::vector<SDL_Rect> rects102;
    std::vector<SDL_Rect> rects103;
    std::vector<SDL_Rect> rects104;
    std::vector<SDL_Rect> rects105;

    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if (cellGrid[col][row].element->type != 0) {
                SDL_Rect tempRect = SDL_Rect();
                tempRect.w = cellWidthPixels;
                tempRect.h = cellHeightPixels;
                tempRect.x = col*cellWidthPixels;
                tempRect.y = row*cellHeightPixels;
                switch (cellGrid[col][row].element->colorID) {
                    case 101:
                        rects101.push_back(tempRect);
                        break;
                    case 102:
                        rects102.push_back(tempRect);
                        break;
                    case 103:
                        rects103.push_back(tempRect);
                        break;
                    case 104:
                        rects104.push_back(tempRect);
                        break;
                    case 105:
                        rects105.push_back(tempRect);
                        break;
                }
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, utils::COLOR_101[0], utils::COLOR_101[1], utils::COLOR_101[2], utils::COLOR_101[3]);
    for (int i = 0; i < (int) rects101.size(); i++) {
        SDL_RenderFillRect(renderer, &rects101[i]);
    }
    SDL_SetRenderDrawColor(renderer, utils::COLOR_102[0], utils::COLOR_102[1], utils::COLOR_102[2], utils::COLOR_102[3]);
    for (int i = 0; i < (int) rects102.size(); i++) {
        SDL_RenderFillRect(renderer, &rects102[i]);
    }
    SDL_SetRenderDrawColor(renderer, utils::COLOR_103[0], utils::COLOR_103[1], utils::COLOR_103[2], utils::COLOR_103[3]);
    for (int i = 0; i < (int) rects103.size(); i++) {
        SDL_RenderFillRect(renderer, &rects103[i]);
    }
    SDL_SetRenderDrawColor(renderer, utils::COLOR_104[0], utils::COLOR_104[1], utils::COLOR_104[2], utils::COLOR_104[3]);
    for (int i = 0; i < (int) rects104.size(); i++)
        SDL_RenderFillRect(renderer, &rects104[i]);
    SDL_SetRenderDrawColor(renderer, utils::COLOR_105[0], utils::COLOR_105[1], utils::COLOR_105[2], utils::COLOR_105[3]);
    for (int i = 0; i < (int) rects105.size(); i++)
        SDL_RenderFillRect(renderer, &rects105[i]);
        
    int endTimeMS = SDL_GetTicks()-startTimeMS;

    /*if (endTimeMS >= 10)
        std::cout << "Lag-spike! Frame time: " << endTimeMS << " milliseconds" << std::endl;*/
    if (time % 10 == 0) {
        std::cout << "Render time: " << endTimeMS << " milliseconds" << std::endl;
    }

    //Shows thread regions
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //for (int i = 0; i < numThreads; i++)
    //    SDL_RenderDrawLine(renderer, (int) (width/numThreads*i*cellWidthPixels), 0, (int) (width/numThreads*i*cellHeightPixels), height*cellHeightPixels);
}

Element* Grid::getElement(int x, int y) {
    return cellGrid[x][y].getElement();
}

void Grid::setElement(int x, int y, Element *element) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        cellGrid[x][y].setElement(element);
    }
}