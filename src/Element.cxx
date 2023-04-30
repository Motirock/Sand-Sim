#include <stdlib.h>

#include "Element.h"

Element::Element(int xPosition, int yPosition, char characterID) : x(xPosition), y(yPosition), charID(characterID) {
    switch (characterID) {
        case '-':
            type = AIR;
            state = EMPTY;
            colorID = 0;
            //R: 0.00
            //G: 0.00
            //B: 0.00
            //A: 0.00
            break;
        case 's':
            type = STONE;
            state = STATIC;
            colorID = 1;
            //R: 0.30
            //G: 0.30
            //B: 0.30
            //A: 1.00
            break;
        case 'S':
            type = SAND;
            state = FALLING;
            colorID = 2;
            //R: 0.75
            //G: 0.70
            //B: 0.00
            //A: 1.00
            break;
        case 'w':
            type = WATER;
            state = LIQUID;
            colorID = 3;
            //R: 0.00
            //G: 0.00
            //B: 0.65
            //A: 0.70
            viscosity = 0.75;
            break;
        case 'l':
            type = LAVA;
            state = LIQUID;
            colorID = 4;
            //R: 0.80
            //G: 0.20
            //B: 0.00
            //A: 1.00
            viscosity = 0.1;
            break;
        case '^':
            type = STEAM;
            state = GAS;
            colorID = 5;
            //R: 0.95
            //G: 0.95
            //B: 0.95
            //A: 0.45
            viscosity = 0.75;
            break;
            
    }
}

Element::Element(int xPosition, int yPosition) : x(xPosition), y(yPosition) {
    charID = '-';
    type = AIR;
    colorID = 0;
}

Element::Element() {
    x = 0;
    y = 0;
    charID = '-';
    type = AIR;
    colorID = 0;
}

Element::~Element() {
}