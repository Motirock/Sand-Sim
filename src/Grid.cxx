#include "Grid.h"

ThreadPool gridThreadPool(10);

void updateRegion(Grid *grid, int ID) {
    int leftLimit = (int) (grid->width/grid->numTasks*ID);
    int rightLimit = (int) (grid->width/grid->numTasks*(ID+1));

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

Grid::Grid(int gridWidth, int gridHeight) : width(gridWidth), height(gridHeight), numThreads(10), numTasks(64)   {
    time = 0;
    pixelsPerCellWidth = 1600.0/(visibleWidth-1);
    pixelsPerCellHeight = 900.0/(visibleHeight-1);

    cellGrid = new Cell*[width];
    for (int col = 0; col < width; col++)
        cellGrid[col] = new Cell[height];
    for (int r = height-1; r >= 0; r--) {
        for (int c = 0; c < width; c++) {
            cellGrid[c][r] = Cell(c, r);
        }
    }

    float xOffset = 2.0f/(visibleWidth-1);
    float yOffset = 2.0f/(visibleHeight-1);
    float vertices[] = {
        //Positions of base quad
        -1.0f,          -1.0f+yOffset,
        -1.0f+xOffset,  -1.0f,
        -1.0f,          -1.0f,

        -1.0f,          -1.0f+yOffset,
        -1.0f+xOffset,  -1.0f,
        -1.0f+xOffset,  -1.0f+yOffset,
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int)*visibleTotal, &colors[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);
    glVertexAttribDivisor(1, 1);

    std::cout << "Grid cell width: " << width << " Grid cell height: " << height << std::endl;
}

Grid::Grid() : width(320), height(180), numThreads(10), numTasks(64)  {
    std::cout << "USED GRID'S DEFAULT CONSTRUCTOR" << std::endl;
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

void Grid::update() {
    time++;

    for (int i  = 0; i < numTasks; i++) {
        gridThreadPool.doJob(std::bind(updateRegion, this, i));
    }
}

void Grid::draw(Shader& shader, int playerX, int playerY) {
	//Bind shader to be able to access uniforms
    int index = 0;
    for (int row = playerY+visibleHeight/2; row >= playerY-visibleHeight/2; row--) {
        for (int col = playerX-visibleWidth/2; col <= playerX+visibleWidth/2; col++) {
            if (col >= 0 && col < width && row >= 0 && row < height)
                colors[index++] = cellGrid[col][row].element->colorID;
            else
                colors[index++] = -1;
        }
    }

	shader.activateShader();
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int)*visibleTotal, &colors[0], GL_DYNAMIC_DRAW);

	// Draw the actual mesh
    glBindVertexArray(vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, visibleTotal);  
    glBindVertexArray(0);
}

Element* Grid::getElement(int x, int y) {
    return cellGrid[x][y].getElement();
}

void Grid::setElement(int x, int y, Element *element) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        element->hasBeenUpdated = true;
        cellGrid[x][y].setElement(element);
        int cellIndexStart = (y*width + x)*4;
    }
}