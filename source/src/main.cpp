//g++ -c src/*.cpp -std=c++14 -pthread -g -Wall -m64 -I include
//g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
//start bin\debug\main
//
//g++ -c src/*.cpp -std=c++14 -pthread -O3 -Wall -m64 -I include
//g++ *.o -o bin/release/main -s -L C:/SDL2-w64/lib -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
//start bin\release\main

//https://lospec.com/palette-list/aap-64

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <thread>

#include "Utils.hpp"
#include "Game.h"
#include "Cell.h"
#include "ThreadPool.hpp"

Game *game = nullptr;

ThreadPool threadPool(1);

void checkInput() {
	game->handleEvents();
}

void updateGame() {
	game->update();
}

int main(int argc, char *args[])
{
game = new Game();
game->init("Sussy Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, false);

const int inputChecksPerSecond = 60;
const int updatesPerSecond = 60;
const int rendersPerSecond = 60;
const float inputChecksIntervalMS = 1000.0f/inputChecksPerSecond;
const float updatesIntervalMS = 1000.0f/updatesPerSecond;
const float rendersIntervalMS = 1000.0f/rendersPerSecond;
long inputChecks = 0;
long updates = 0;
long renders = 0;
while (game->checkIfRunning()) {	
	long currentTimeMS = utils::hireTimeInMilliseconds();

	bool shouldCheckInput = inputChecks*inputChecksIntervalMS <= currentTimeMS;
	bool shouldUpdate = updates*updatesIntervalMS <= currentTimeMS; 
	bool shouldRender = renders*rendersIntervalMS <= currentTimeMS;

	if (shouldCheckInput) {
		//threadPool.doJob(std::bind(checkInput));
		game->handleEvents();
		inputChecks++;
	}
	if (shouldUpdate) {
		threadPool.doJob(std::bind(updateGame));
		updates++;
	}
	if (shouldRender) {
		game->render();
		renders++;
	}

	//If needed to render two frames, still only renders once
	if (shouldRender && (renders+5)*rendersIntervalMS <= currentTimeMS)
		renders++;

	SDL_Delay(0.001);
}

game->clean();

return 0;
}











