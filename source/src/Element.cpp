#include <stdlib.h>

#include "Element.h"

Element::Element(int xPosition, int yPosition, char characterID) : x(xPosition), y(yPosition), charID(characterID) {
    colorRGB = new int[3];
    switch (characterID) {
        case '-':
            type = AIR;
            state = EMPTY;
            colorRGB[0] = 16;
            colorRGB[1] = 16;
            colorRGB[2] = 16;
            break;
        case 's':
            type = STONE;
            state = STATIC;
            colorRGB[0] = rand()%11+70;
            colorRGB[1] = rand()%11+70;
            colorRGB[2] = rand()%11+70;
            break;
        case 'S':
            type = SAND;
            state = FALLING;
            colorRGB[0] = rand()%41+180;
            colorRGB[1] = rand()%41+180;
            colorRGB[2] = 0;
            break;
        case 'w':
            type = WATER;
            state = LIQUID;
            colorRGB[0] = 0;
            colorRGB[1] = 0;
            colorRGB[2] = rand()%11+215;
            viscosity = 0.75;
            break;
         case 'l':
            type = LAVA;
            state = LIQUID;
            colorRGB[0] = rand()%21+190;
            colorRGB[1] = rand()%41+30;
            colorRGB[2] = 0;
            viscosity = 0.05;
            break;
        case '^':
            type = STEAM;
            state = GAS;
            colorRGB[0] = rand()%11+235;
            colorRGB[1] = rand()%11+235;
            colorRGB[2] = rand()%11+235;
            viscosity = 0.75;
            break;
            
    }
}

Element::Element(int xPosition, int yPosition) : x(xPosition), y(yPosition) {
    charID = '-';
    type = AIR;
    colorRGB = new int[3];
    colorRGB[0] = 16;
    colorRGB[1] = 16;
    colorRGB[2] = 16;
}

Element::Element() {
    x = 0;
    y = 0;
    charID = '-';
    type = AIR;
    colorRGB = new int[3];
    colorRGB[0] = 16;
    colorRGB[1] = 16;
    colorRGB[2] = 16;
}

Element::~Element() {
    free(colorRGB);
}