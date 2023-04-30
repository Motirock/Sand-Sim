#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <cmath>

#include "Grid.h"
#include "ShaderClass.h"
#include "Utils.hxx"

class Game {
public:
    Game();
    ~Game();

    int init(int screenWidth, int screenHeight, const char *windowName, bool fullscreen);
    void input();
    void update();
    void draw();
    void clean();
    bool shouldClose();

    int playerX = 0, playerY = 0;
    GLFWwindow *window = nullptr;
private:
    Grid *grid = nullptr;
    int screenWidth, screenHeight;
    bool fullscreen;
    Shader *gridShader = nullptr;

    char selectedElement = 'S';
    bool paused = false;

    unsigned int MSAASamples = 8;
};

#endif