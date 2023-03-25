#include <stdlib.h>

#include "Element.h"

Element::Element(int xPosition, int yPosition, char characterID) : x(xPosition), y(yPosition), charID(characterID) {
    colorRGB = new int[3];
    switch (characterID) {
        case '-':
            type = EMPTY;
            colorRGB[0] = 16;
            colorRGB[1] = 16;
            colorRGB[2] = 16;
            break;
        case 's':
            type = STONE;
            colorRGB[0] = rand()%11+70;
            colorRGB[1] = rand()%11+70;
            colorRGB[2] = rand()%11+70;
            break;
        case 'S':
            type = SAND;
            colorRGB[0] = rand()%41+180;
            colorRGB[1] = rand()%41+180;
            colorRGB[2] = 0;
            break;
        case 'w':
            type = WATER;
            colorRGB[0] = 0;
            colorRGB[1] = 0;
            colorRGB[2] = rand()%11+215;
            viscosity = 0.75;
            break;
            
    }
}

Element::Element(int xPosition, int yPosition) : x(xPosition), y(yPosition) {
    charID = '-';
    type = EMPTY;
    colorRGB = new int[3];
    colorRGB[0] = 16;
    colorRGB[1] = 16;
    colorRGB[2] = 16;
}

Element::Element() {
    x = 0;
    y = 0;
    charID = '-';
    type = EMPTY;
    colorRGB = new int[3];
    colorRGB[0] = 16;
    colorRGB[1] = 16;
    colorRGB[2] = 16;
}

Element::~Element() {
    free(colorRGB);
}