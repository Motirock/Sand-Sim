#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <iostream>

namespace utils {
    inline float hireTimeInSeconds() {
        return SDL_GetTicks()*0.001f;
    }

    inline float hireTimeInMilliseconds() {
         return SDL_GetTicks();
    }

    inline int** getPointsInCircle(int centerX, int centerY, int radius) {
        //std::cout << "Creating circle at (" << centerX << ", " << centerY << ")" << std::endl;
        int size = 2*radius+1;
        int** points = new int*[size];
        for(int i = 0; i < size; i++) {
            points[i] = new int[size*2];
            for(int j = 0; j < size*2; j++) {
                points[i][j] = -1;
            }
        }
        int px, py;
        for(int x = centerX-radius; x <= centerX+radius; x++) {
            for(int y = centerY-radius; y <= centerY+radius; y++) {
                px = x-centerX;
                py = y-centerY;
                if(px*px + py*py <= radius*radius) {
                    points[x-centerX+radius][(y-centerY+radius)*2] = px+centerX;
                    points[x-centerX+radius][(y-centerY+radius)*2+1] = py+centerY;
                }
            }
        }
        return points;
    }
}