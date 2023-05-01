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
#include "Input.h"
#include "Player.h"

class Game {
public:
    Game();
    ~Game();

    int init(int screenWidth, int screenHeight, const char *windowName, bool fullscreen);
    void handleInput();
    void update();
    void draw();
    void clean();
    bool shouldClose();

    GLFWwindow *window = nullptr;
private:
    Grid *grid = nullptr;
    int screenWidth, screenHeight;
    bool fullscreen;
    Shader *gridShader = nullptr;

    char selectedElement = 'S';
    bool paused = false;
    Input input;

    Player player;

    unsigned int MSAASamples = 8;
};

#endif