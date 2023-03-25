#include <iostream>

#include "Cell.h"
#include "Element.h"
#include "Grid.h"

Grid::Grid(int gridWidth, int gridHeight)  : width(gridWidth), height(gridHeight) {
    cellGrid = new Cell*[width];
        for (int col = 0; col < width; col++)
            cellGrid[col] = new Cell[height];
        for (int col = 0; col < width; col++)
            for (int row = 0; row < height; row++)
                cellGrid[col][row] = Cell(col, row);
}

Grid::Grid() : width(100), height(100) {
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
    int count = 0;
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            count++;
            if (cellGrid[col][row].element->type == 2) {
                if (row+1 < height) {
                    cellGrid[col][row+1].setElement(cellGrid[col][row].element);
                    cellGrid[col][row].setElement(new Element(col, row, '-'));
                }
            }
        }
    }
}

void Grid::render(SDL_Renderer *renderer) {
    SDL_Rect tempRect;
    tempRect.w = 10;
    tempRect.h = 10;
    Cell tempCell;
    Element tempElement;
   // std::cout << grid[0][0].colorRGB[0] << std::endl;
    for (int col = 0; col < 160; col++) {
        for (int row = 0; row < 90; row++) {
            tempCell = cellGrid[col][row];
            tempElement = *tempCell.element;
            tempRect.x = tempCell.x*10;
            tempRect.y = tempCell.y*10;
            SDL_SetRenderDrawColor(renderer, tempElement.colorRGB[0], tempElement.colorRGB[1], tempElement.colorRGB[2], 255);
            SDL_RenderFillRect(renderer, &tempRect);
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

Element* Grid::getElement(int x, int y) {
    return cellGrid[x][y].getElement();
}

void Grid::setElement(int x, int y, Element *element) {
    cellGrid[x][y].setElement(element);
}