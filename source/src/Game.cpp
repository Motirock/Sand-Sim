#include <iostream>

#include "Cell.h"
#include "Element.h"
#include "Entity.h"
#include "Game.h"
#include "TextureManager.h"
#include "Utils.hpp"

Entity *player;
SDL_Event Game::event;

Game::Game() {
    std::cout << "Game object created" << std::endl;
}

Game::~Game() {

}

void Game::init(const char *title, int screenX, int screenY, int screenWidth, int screenHeight, bool fullscreen) {
    int flags = 0;
    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;


    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Successful subsystem initialization" << std::endl;
        window = SDL_CreateWindow(title, screenX, screenY, screenWidth, screenHeight, flags);

        if (window)
            std::cout << "Successful window creation" << std::endl;
        else {
            std::cout << "Window creation failed!" << std::endl;
            isRunning = false;
            return;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        if (renderer)
            std::cout << "Successful renderer creation" << std::endl;
        else {
            std::cout << "Renderer creation failed" << std::endl;
            isRunning = false;
            return;
        }

        isRunning = true;
    }
    else {
        std::cout << "Subsystem initialization failed!" << std::endl;
        isRunning = false;
        return;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 
    player = new Entity("res/gfx/ground_grass_1.png", renderer, 50, 50);
    //Creates grid
    grid = new Grid(320, 180);
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        upPressed = true;
                        std::cout<<"UP" << std::endl;
                        break;
                    case SDLK_s:
                        downPressed = true;
                        break;
                    case SDLK_a:
                        leftPressed = true;
                        break;
                    case SDLK_d:
                        rightPressed = true;
                        break;
                    case SDLK_1:
                        elementSelected = 1;
                        break;
                    case SDLK_2:
                        elementSelected = 2;
                        break;
                    case SDLK_3:
                        elementSelected = 3;
                        break;
                    case SDLK_4:
                        elementSelected = 4;
                        break;
                    case SDLK_5:
                        elementSelected = 5;
                        break;
                    case SDLK_6:
                        elementSelected = 6;
                        break;
                    case SDLK_7:
                        elementSelected = 7;
                        break;
                    case SDLK_8:
                        elementSelected = 8;
                        break;
                    case SDLK_9:
                        elementSelected = 9;
                        break;
                    case SDLK_0:
                        elementSelected = 10;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        upPressed = false;
                        std::cout<<"UP RELEASED" << std::endl;
                        break;
                    case SDLK_s:
                        downPressed = false;
                        break;
                    case SDLK_a:
                        leftPressed = false;
                        break;
                    case SDLK_d:
                        rightPressed = false;
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        mouseLeftPressed = true;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        mouseLeftPressed = false;
                        break;
                }
                break;
            default:
                break;
        }
    }
};

void Game::update() {
    if (mouseLeftPressed) {
        char elementSelectedChar;
        switch (elementSelected) {
            case 1:
                elementSelectedChar = 's';
                break;
            case 2:
                elementSelectedChar = 'S';
                break;
            case 3:
                elementSelectedChar = 'w';
                break;
            case 4:
                elementSelectedChar = 'l';
                break;
            case 5:
                elementSelectedChar = '^';
                break;
            default:
                elementSelectedChar = '-';
        }

        int drawRadius = 4;
        int** points = utils::getPointsInCircle((int) (mouseX/grid->cellWidthPixels), (int) (mouseY/grid->cellHeightPixels), drawRadius);
        int size = 2*drawRadius+1;
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < size*2; j += 2) {
                if (points[i][j] >= 0)
                    grid->setElement(points[i][j], points[i][j+1], new Element(points[i][j], points[i][j+1], elementSelectedChar));
            }
        }
    }
    grid->update();
    count++;
};

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    //SDL_RenderCopy(renderer, playerTexture, NULL, NULL);
    //player->render();

    grid->render(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);
};

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
};

