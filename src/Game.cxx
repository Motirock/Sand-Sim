#include "Game.h"

std::string parentDirectory = (std::filesystem::current_path().std::filesystem::path::parent_path()).string();

Game::Game() {
    std::cout << "Game constructed" << std::endl;
}

Game::~Game() {

}

int Game::init(int screenWidth, int screenHeight, const char *windowName, bool fullscreen) {
    std::cout << "Game initialization started" << std::endl;
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_SAMPLES, MSAASamples);
    std::cout << "GLFW initialized" << std::endl;

	if (!fullscreen)
		window = glfwCreateWindow(screenWidth, screenHeight, "Sussy Engine", NULL, NULL);
	else {
		screenWidth = 2560;
		screenHeight = 1440;
		window = glfwCreateWindow(screenWidth, screenHeight, "Sussy Engine", glfwGetPrimaryMonitor(), NULL);
	}

	if (window == NULL) {
		std::cout << "GLFW Window creation failed!" << std::endl;
		glfwTerminate();
		return 1; 
	}
    std::cout << "Window creation successful" << std::endl;
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Specify the color of the background
	glClearColor(0.06f, 0.06f, 0.06f, 1.0f);

    grid = new Grid(640, 320);
    std::cout << "Grid creation successful" << std::endl;

    gridShader = new Shader("./shaders/grid.vert", "./shaders/grid.frag");
    std::cout << "Grid Shader construction successful" << std::endl;

    std::cout << "Game initialization successful" << std::endl;
    return 0;
}

void Game::input() {
    //Take care of all GLFW events
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        playerY--;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        playerY++;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerX--;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerX++;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        paused = true;
    }
    else {
        paused = false;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        selectedElement = 's';
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        selectedElement = 'S';
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        selectedElement = 'w';
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        selectedElement = 'l';
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        selectedElement = '^';
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        selectedElement = '-';
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double mouseX = 0, mouseY = 0;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        int col = (int) (mouseX/grid->pixelsPerCellWidth+playerX-grid->visibleWidth/2);
        int row = (int) (mouseY/grid->pixelsPerCellHeight+playerY-grid->visibleHeight/2);
        int drawRadius = 3;
        int** points = utils::getPointsInCircle(col, row, drawRadius);
        int size = 2*drawRadius+1;
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < size*2; j += 2) {
                if (points[i][j] >= 0)
                    grid->setElement(points[i][j], points[i][j+1], new Element(points[i][j], points[i][j+1], selectedElement));
            }
        }
    }
}

void Game::update() {
    if (!paused) {
        grid->update();
    }
}

void Game::draw() {
    //Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    grid->draw(*gridShader, playerX, playerY);

    //Swap the back buffer with the front buffer
    glfwSwapBuffers(window);
}

void Game::clean() {
    gridShader->deleteShader();
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Game::shouldClose() {
    return glfwWindowShouldClose(window);
}