#pragma once

class Element { 
public:
    Element(int xPosition, int yPosition, char characterID);
    Element(int xPosition, int yPosition);
    Element();
    ~Element();
    float x, y, vx, vy, viscosity;
    enum Type {AIR = 0, STONE = 1, SAND = 2, WATER = 3, STEAM = 4};
    enum State {EMPTY = 0, STATIC = 1, FALLING = 2, LIQUID = 3, GAS = 4};
    Type type;
    State state;
    char charID;
    int* colorRGB;
    bool hasBeenUpdated;
};