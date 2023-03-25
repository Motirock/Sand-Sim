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

const int inputChecksPerSecond = 100;
const int updatesPerSecond = 50;
const int rendersPerSecond = 50;
const float inputChecksIntervalMS = 1000.0f/inputChecksPerSecond;
const float updatesIntervalMS = 1000.0f/updatesPerSecond;
const float rendersIntervalMS = 1000.0f/rendersPerSecond;
long inputChecks = 0;
long updates = 0;
long renders = 0;
while (game->checkIfRunning()) {	
	float currentTimeMS = utils::hireTimeInMilliseconds();
	//std::cout << currentTimeMS << " " << updates*updatesIntervalMS << std::endl;
	if (inputChecks*inputChecksIntervalMS <= currentTimeMS) {
		game->handleEvents();
		inputChecks++;
	}
	if (updates*updatesIntervalMS <= currentTimeMS) {
		game->update();
		updates++;
	}
	if (renders*rendersIntervalMS <= currentTimeMS) {
		game->render();
		renders++;
	}
	SDL_Delay(0.1);
}

game->clean();

return 0;
}