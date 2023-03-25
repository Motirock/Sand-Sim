#pragma once

class Element { 
public:
    Element(int xPosition, int yPosition, char characterID);
    Element(int xPosition, int yPosition);
    Element();
    ~Element();
    float x, y, vx, vy;
    enum Type {EMPTY = 0, STONE = 1, SAND = 2};
    Type type;
    char charID;
    int* colorRGB;
};