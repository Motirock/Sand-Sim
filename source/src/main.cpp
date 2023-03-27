//g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include
//g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
//start bin\debug\main
//
//g++ -c src/*.cpp -std=c++14 -pthread -O3 -Wall -m64 -I include
//g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
//start bin\release\main

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <thread>

#include "Utils.hpp"
#include "Game.h"
#include "Cell.h"

Game *game = nullptr;

void updateGame() {
	game->update();
}

int main(int argc, char *args[])
{
game = new Game();
game->init("Sussy Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, false);

const int inputChecksPerSecond = 200;
const int updatesPerSecond = 100;
const int rendersPerSecond = 50;
const float inputChecksIntervalMS = 1000.0f/inputChecksPerSecond;
const float updatesIntervalMS = 1000.0f/updatesPerSecond;
const float rendersIntervalMS = 1000.0f/rendersPerSecond;
long inputChecks = 0;
long updates = 0;
long renders = 0;
std::thread *updateThread;
while (game->checkIfRunning()) {	
	float currentTimeMS = utils::hireTimeInMilliseconds();

	bool shouldCheckInput = inputChecks*inputChecksIntervalMS <= currentTimeMS;
	bool shouldUpdate = currentTimeMS > 3000/*Three Second buffer*/ && updates*updatesIntervalMS <= currentTimeMS; 
	bool shouldRender = renders*rendersIntervalMS <= currentTimeMS;

	//if (shouldCheckInput && shouldUpdate && shouldRender) {}

	if (inputChecks*inputChecksIntervalMS <= currentTimeMS) {
		game->handleEvents();
		inputChecks++;
	}
	if (currentTimeMS > 3000/*Three Second buffer*/ && updates*updatesIntervalMS <= currentTimeMS) {
		updateThread = new std::thread(updateGame);
		updateThread->join();
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