#include <stdlib.h>
#include <iostream>

#include "Element.h"

Element::Element(int xPosition, int yPosition, char characterID) : x(xPosition), y(yPosition), charID(characterID) {
    switch (characterID) {
        case '-':
            type = AIR;
            state = EMPTY;
            colorID = 1;
            break;
        case 's':
            type = STONE;
            state = STATIC;
            colorID = 101;
            break;
        case 'S':
            type = SAND;
            state = FALLING;
            colorID = 102;
            break;
        case 'w':
            type = WATER;
            state = LIQUID;
            colorID = 103;
            viscosity = 0.75;
            break;
         case 'l':
            type = LAVA;
            state = LIQUID;
            colorID = 104;
            viscosity = 0.05;
            break;
        case '^':
            type = STEAM;
            state = GAS;
            colorID = 105;
            viscosity = 0.75;
            break;
    }
}

Element::Element(int xPosition, int yPosition) : x(xPosition), y(yPosition) {
    charID = '-';
    type = AIR;
    colorID = 1;
}

Element::Element() {
    x = 0;
    y = 0;
    charID = '-';
    type = AIR;
    colorID = 1;
}

Element::~Element() {
}