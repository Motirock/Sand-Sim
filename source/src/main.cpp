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

Game *game = nullptr;

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
std::thread *updateThread;
while (game->checkIfRunning()) {	
	float currentTimeMS = utils::hireTimeInMilliseconds();

	bool shouldCheckInput = inputChecks*inputChecksIntervalMS <= currentTimeMS;
	bool shouldUpdate = /*currentTimeMS > 3000Three Second buffer && */updates*updatesIntervalMS <= currentTimeMS; 
	bool shouldRender = renders*rendersIntervalMS <= currentTimeMS;

	if (shouldCheckInput) {
		game->handleEvents();
		inputChecks++;
	}
	if (shouldUpdate && shouldRender) {
		updateThread = new std::thread(updateGame);
		game->render();
		updateThread->join();
		updates++;
		renders++;
	}
	else if (shouldUpdate) {
		updateThread = new std::thread(updateGame);
		updateThread->join();
		updates++;
	}
	else if (shouldRender) {
		game->render();
		renders++;
	}

	SDL_Delay(0.001);
}

game->clean();

return 0;
}

/*
ID: 1073752066   Could not get rect group because the colorID was invalid
 ID: 1073752066   Could not get rect group because the colorID was invalid
 ID: 1073752066 ID: 895944096   Could not get rect group because the colorID was invalid ID:  ID:  ID: 8959440961073752066
      895944096 ID:     ID:  ID: 1073752066Could not get rect group because the colorID was invalid
   Could not get rect
    group because the colorID was invalid
1073752066   895944096Could not get rect group because the colorID was invalid   Could not get rect group because the colorID was invalidCould not get rect group because the colorID was invalid
Could not get rect group because the colorID was invalid

Could not get rect group because the colorID was invalid
*/

/*
ID:  ID: 1073752066   Could not get rect group because the colorID was invalid
 ID: 1073752066 ID:    Could not get rect group because the colorID was invalid
 ID: 1073752066   Could not get rect group because the colorID was invalid
 ID: 1073752066   Could not get rect group because the colorID was invalid
 ID: 1073752066   1073752066   Could not get rect group because the colorID was invalid
 ID: 1073752066    ID: 1073752066   Could not get rect group because the colorID was invalid
 ID: 1073752066   Could not get rect group because the colorID was invalid
Could not get rect group because the colorID was invalid
1073752066   Could not get rect group because the colorID was invalid
Could not get rect group because the colorID was invalid
*/