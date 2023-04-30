#ifndef CELL_CLASS_H
#define CELL_CLASS_H

#include "Element.h"

class Cell {
public:
    Cell(int xPosition, int yPosition, char elementCharacterID);
    Cell(int xPosition, int yPosition);
    Cell(char elementCharacterID);
    Cell();
    ~Cell();
    Element* getElement();
    void setElement(Element *newElement);
    int x, y;
    Element *element;
};


#endif