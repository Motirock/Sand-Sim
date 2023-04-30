/*
g++ -c -std=c++17 src/*.cxx -Iinclude
g++ *.o -o bin/main -Llib -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit
./bin/main
*/

//https://lospec.com/palette-list/aap-64

#include "Game.h"
#include "Utils.hxx"

int main() {
	Game *game = new Game();
	game->init(1600, 900, "Sussy Engine", false);

	//Game loop
	long inputChecks = 0, updates = 0, draws = 0;
	float inputCheckDelay = 1.0f/50.0f, updateDelay = 1.0f/50.0f, drawDelay = 1.0f/60.0f;
	int thisSecond = 1, updatesThisSecond = 0, drawsThisSecond = 0;
	while (!game->shouldClose()) {
		double crntTime = glfwGetTime();

		if (crntTime >= inputChecks*inputCheckDelay) {
			inputChecks++;
			game->input();
		}

		if (crntTime >= updates*updateDelay) {
			updates++;
			updatesThisSecond++;
			game->update();
		}

		if (crntTime >= draws*drawDelay) {
			draws++;
			drawsThisSecond++;
			game->draw();
		}

		if ((int) crntTime > thisSecond) {
			thisSecond = (int) crntTime;
			std::cout << "TPS: " << updatesThisSecond << "\tFPS: " << drawsThisSecond << std::endl;
			updatesThisSecond = 0;
			drawsThisSecond = 0;
		}
	}

	//Cleaning up
	game->clean();
	return 0;
}