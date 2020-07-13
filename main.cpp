/*
	Project Name: SNES-Style Driving Prototype
	Dev: Hunter Werenskjold
	Date: 10/1/19

	Description:
		Out of curiosity, I wanted to see what would go into making a psuedo-3D real-time renderer for a driving game.
*/

#include <iostream>
#include <vector>
#include <time.h>

#include "gfxHelper.h"
#include "consoleColorer.h"

#include "road.h"
#include "player.h"

#include "definitions.h"

enum ObjectShape
{
	CIRCLE,
	SQUARE
};

static struct Camera {
	rn::vector3f v;
	float depth;
} camera;

SDL_Window* window;
SDL_Renderer* renderer;

std::shared_ptr<Road> road;

std::shared_ptr<Player> player;

std::vector<std::shared_ptr<Segment>> segments;
int segmentsPassed = 0;

float iTime = 0.f;

// Testing-related
clock_t cycleTimeStart = 0.f, cycleTimeEnd = 0.f;

bool isRunning = true;

void updateInputs(SDL_Event& e);
void update();
void draw();

void initialize();

int main(int args, char* argv[])
{
	initialize();

	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);

	SDL_Event e;
	while (isRunning)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				isRunning = false;
			}

			player->updateInputs(e);
		}

		update();

		draw();

		iTime += DELTA_TIME;
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}

// Check clipping of objects and segments
void checkClipping()
{
	// Clip Objects
}

void update()
{

	// Object Clipping
	checkClipping();

	road->update(DELTA_TIME);

	player->update(DELTA_TIME);

	camera.v.z = player->dV.wV.z - camera.depth * camera.v.y;
	camera.v.x = player->dV.wV.x;
	
}

void draw()
{
	rn::project(player->dV, camera.v, camera.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	
	road->draw(renderer, camera.v, camera.depth);

	rn::dualVector floatingDot(rn::vector3f(0, 0, 1000));
	rn::project(floatingDot, camera.v, camera.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

	if (floatingDot.wV.z > player->dV.wV.z) {
		SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
		gfxDrawBrenCircle(renderer, floatingDot.sV.x, floatingDot.sV.y, 10, true);
	}

	player->draw(renderer, camera.v, camera.depth);

	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);
}

void initialize()
{
	// Create Window and Begin
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		std::cout << INLINE_COLOR_FONT("Subsystems Initialized", FONT_GREEN) << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Subsystems", "FATAL", "FAILED to initialize");
		isRunning = false;
	}

	window = SDL_CreateWindow("Driving Game - Hunter Werenskjold", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window)
		std::cout << INLINE_COLOR_FONT("Window Created", FONT_GREEN) << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Window", "FATAL", "Failed to be created");
		isRunning = false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer)
		std::cout << INLINE_COLOR_FONT("Renderer Created", FONT_GREEN) << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Renderer", "FATAL", "Failed to be created");
		isRunning = false;
	}

	road = std::make_shared<Road>();

	camera.v.x = 0;
	camera.v.y = CAMERA_HEIGHT;
	camera.v.z = 0;
	camera.depth = 1.f / tanf((CAMERA_FOV / 2.f) * PI / 180.f);

	player = std::make_shared<Player>(rn::vector3f(0), camera.depth, camera.v.y);
}