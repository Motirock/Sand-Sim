#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <iostream>

namespace utils {
    inline float hireTimeInSeconds() {
        return SDL_GetTicks()*0.001f;
    }

    inline long hireTimeInMilliseconds() {
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

    /*inline void setRenderColorFromColor(SDL_Renderer *renderer, int colorID) {
        switch (colorID) {
            case 101:
                SDL_SetRenderDrawColor(renderer, COLOR_101->r, COLOR_101->b, COLOR_101->g, COLOR_101->a);
                break;
            case 102:
                SDL_SetRenderDrawColor(renderer, COLOR_102->r, COLOR_102->b, COLOR_102->g, COLOR_102->a);
                break;
            case 103:
                SDL_SetRenderDrawColor(renderer, COLOR_103->r, COLOR_103->b, COLOR_103->g, COLOR_103->a);
                break;
            case 104:
                SDL_SetRenderDrawColor(renderer, COLOR_104->r, COLOR_104->b, COLOR_104->g, COLOR_104->a);
                break;
            case 105:
                SDL_SetRenderDrawColor(renderer, COLOR_105->r, COLOR_105->b, COLOR_105->g, COLOR_105->a);
                break;
            default:
                std::cout << "Could not change renderer color as colorID given was invlaid" << std::endl;
        }
    }*/

    /*Color *COLOR_1 = new Color(5, 5, 5, 255);
    Color *COLOR_2 = new Color(20, 15, 20, 255);
    Color *COLOR_3 = new Color(60, 25, 35, 255);
    Color *COLOR_4 = new Color(115, 25, 45, 255);
    Color *COLOR_5 = new Color(180, 32, 42, 255);
    Color *COLOR_6 = new Color(115, 25, 45, 255);
    Color *COLOR_7 = new Color(115, 25, 45, 255);
    Color *COLOR_8 = new Color(115, 25, 45, 255);
    Color *COLOR_9 = new Color(115, 25, 45, 255);
    Color *COLOR_10 = new Color(115, 25, 45, 255);
    Color *COLOR_11 = new Color(115, 25, 45, 255);
    Color *COLOR_12 = new Color(115, 25, 45, 255);
    Color *COLOR_13 = new Color(115, 25, 45, 255);
    Color *COLOR_14 = new Color(115, 25, 45, 255);
    Color *COLOR_15 = new Color(115, 25, 45, 255);
    Color *COLOR_16 = new Color(115, 25, 45, 255);
    Color *COLOR_17 = new Color(115, 25, 45, 255);
    Color *COLOR_18 = new Color(115, 25, 45, 255);
    Color *COLOR_19 = new Color(115, 25, 45, 255);
    Color *COLOR_20 = new Color(115, 25, 45, 255);
    Color *COLOR_21 = new Color(115, 25, 45, 255);
    Color *COLOR_22 = new Color(115, 25, 45, 255);
    Color *COLOR_23 = new Color(115, 25, 45, 255);
    Color *COLOR_24 = new Color(115, 25, 45, 255);
    Color *COLOR_25 = new Color(115, 25, 45, 255);
    Color *COLOR_26 = new Color(115, 25, 45, 255);
    Color *COLOR_27 = new Color(115, 25, 45, 255);
    Color *COLOR_28 = new Color(115, 25, 45, 255);
    Color *COLOR_29 = new Color(115, 25, 45, 255);
    Color *COLOR_30 = new Color(115, 25, 45, 255);
    Color *COLOR_31 = new Color(115, 25, 45, 255);
    Color *COLOR_32 = new Color(115, 25, 45, 255);
    Color *COLOR_33 = new Color(115, 25, 45, 255);
    Color *COLOR_34 = new Color(115, 25, 45, 255);
    Color *COLOR_35 = new Color(115, 25, 45, 255);
    Color *COLOR_36 = new Color(115, 25, 45, 255);
    Color *COLOR_37 = new Color(115, 25, 45, 255);
    Color *COLOR_38 = new Color(115, 25, 45, 255);
    Color *COLOR_39 = new Color(115, 25, 45, 255);
    Color *COLOR_40 = new Color(115, 25, 45, 255);
    Color *COLOR_41 = new Color(115, 25, 45, 255);
    Color *COLOR_42 = new Color(115, 25, 45, 255);
    Color *COLOR_43 = new Color(115, 25, 45, 255);
    Color *COLOR_44 = new Color(115, 25, 45, 255);
    Color *COLOR_45 = new Color(115, 25, 45, 255);
    Color *COLOR_46 = new Color(115, 25, 45, 255);
    Color *COLOR_47 = new Color(115, 25, 45, 255);
    Color *COLOR_48 = new Color(115, 25, 45, 255);
    Color *COLOR_49 = new Color(115, 25, 45, 255);
    Color *COLOR_50 = new Color(115, 25, 45, 255);
    Color *COLOR_51 = new Color(115, 25, 45, 255);
    Color *COLOR_52 = new Color(115, 25, 45, 255);
    Color *COLOR_53 = new Color(115, 25, 45, 255);
    Color *COLOR_54 = new Color(115, 25, 45, 255);
    Color *COLOR_55 = new Color(115, 25, 45, 255);
    Color *COLOR_56 = new Color(115, 25, 45, 255);
    Color *COLOR_57 = new Color(115, 25, 45, 255);
    Color *COLOR_58 = new Color(115, 25, 45, 255);
    Color *COLOR_59 = new Color(115, 25, 45, 255);
    Color *COLOR_60 = new Color(115, 25, 45, 255);
    Color *COLOR_61 = new Color(115, 25, 45, 255);
    Color *COLOR_62 = new Color(115, 25, 45, 255);
    Color *COLOR_63 = new Color(115, 25, 45, 255);
    Color *COLOR_64 = new Color(115, 25, 45, 255);*/

    //TEMP
    const int COLOR_101[4] = {75, 75, 75, 255};
    const int COLOR_102[4] = {200, 200, 0, 255};
    const int COLOR_103[4] = {0, 0, 220, 255};
    const int COLOR_104[4] = {200, 50, 0, 255};
    const int COLOR_105[4] = {245, 245, 245, 255};
}