#include <iostream>

#include "Cell.h"
#include "Element.h"
#include "Game.h"

Cell::Cell(int xPosition, int yPosition, char elementCharacterID) : x(xPosition), y(yPosition) {
    element = new Element(xPosition, yPosition, elementCharacterID);
}

Cell::Cell(int xPosition, int yPosition) : x(xPosition), y(yPosition) {
    element = new Element(xPosition, yPosition);
}

Cell::Cell(char elementCharacterID) {
    x = 0;
    y = 0;
    element = new Element(0, 0, elementCharacterID);
}

Cell::Cell() {
    x = 0;
    y = 0;
    element = new Element(0, 0);
}

Cell::~Cell() {
    free(element);
}

Element* Cell::getElement() {
    return element;
}

void Cell::setElement(Element *newElement) {
    this->element = newElement;
}