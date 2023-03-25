#pragma once

#include <SDL2/SDL.h>

#include "Grid.h"

class Game {
public:
    Game();
    ~Game();

    void init(const char *title, int screenX, int screenY, int screenWidth, int screenHeight, bool fullscreen);

    void handleEvents();

    void update();

    void render();

    void clean();

    bool checkIfRunning() {
        return isRunning;
    };
    Grid *grid;
    static SDL_Event event;
    bool upPressed, downPressed, leftPressed, rightPressed;
    bool mouseLeftPressed;
    int mouseX, mouseY;
    int elementSelected;
    long int count = 0;
private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
};