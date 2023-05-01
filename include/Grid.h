#ifndef GRID_CLASS_H
#define GRID_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <thread>
#include <vector>

#include "Cell.h"
#include "Element.h"
#include "Mesh.h"
#include "ThreadPool.hxx"
#include "Camera.h"

class Grid {
public:
    Grid(int gridWidth, int gridHeight);
    Grid();

    void update(int playerX, int playerY);
    void updateCell(int x, int y, int randomNumber);
    void draw(Shader& shader, float playerX, float playerY);
    Element* getElement(int x, int y);
    void setElement(int x, int y, Element *element);
    const int width, height, numThreads, numTasks;
    float pixelsPerCellWidth, pixelsPerCellHeight;
    Cell** cellGrid;
    int time;
    bool running;
    int visibleWidth = 321, visibleHeight = 181;
    int visibleTotal = visibleWidth*visibleHeight;
    glm::vec2 lightPositions[60000];
    int colors[58101];
    unsigned int vao, vbo;
    unsigned int instanceVBO;
    int simulatedWidth = 1280, simulatedHeight = 720;
};

#endif