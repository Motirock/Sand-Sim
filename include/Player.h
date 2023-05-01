#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include <iostream>

#include "Input.h"

class Player {
public:
    Player();
    ~Player();

    void update(Input &input);
    void draw();

    float x = 0.0f, y = 0.0f, speed = 1.5f;
};

#endif