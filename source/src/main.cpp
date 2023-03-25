//g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include
//g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
//start bin\debug\main
//
//g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include
//g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
//start bin\release\main

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Utils.hpp"
#include "Game.h"
#include "Cell.h"

Game *game = nullptr;
Cell test;

int main(int argc, char *args[])
{
std::cout<< "3" << std::endl;
game = new Game();
game->init("Sussy Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, false);

const float timeStep = 1/100.0f;
float accumulator = 0.0f;
float currentTime = utils::hireTimeInSeconds();
while (game->checkIfRunning())
{	
	int startTicks = SDL_GetTicks();
	float newTime = utils::hireTimeInSeconds();
	float frameTime = newTime-currentTime;
	if (frameTime > 0.25f)
		frameTime = 0.25f;
	currentTime = newTime;
	accumulator += frameTime;
	
	while (accumulator >= timeStep) {
		// Get our controls and events
		game->handleEvents();
		accumulator -= timeStep;
	}

	game->update();
	game->render();

	int frameTicks = SDL_GetTicks()-startTicks;
	if (frameTicks < 1000/60.0)
		SDL_Delay(1000/60.0-frameTicks);
}

game->clean();

return 0;
}