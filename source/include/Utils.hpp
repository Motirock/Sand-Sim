#pragma once

#include <SDL2/SDL.h>

namespace utils {
    inline float hireTimeInSeconds() {
        return SDL_GetTicks()*0.001f;
    }

    inline float hireTimeInMilliseconds() {
         return SDL_GetTicks();
    }
}