/*
	Project Name: SNES-Style Driving Prototype
	Dev: Hunter Werenskjold
	Date: 10/1/19

	Description:
		This was a small project from 2019 that I have recently (July, 2020) decided to return to.
			The goal is to create a base for SNES-styled driving games, starting with basic rendering and forward-only progression
				As far as design goes, I'm being a bit loose, so expect rash sudden changes, sorry
				If this project gets a large enough code-base going, I will start working on experimental branches before major revisions are made
*/

#include <iostream>
#include <vector>
#include <time.h>

#include "gfxHelper.h"
#include "consoleColorer.h"

#include "road.h"
#include "player.h"
#include "camera.h"

#include "definitions.h"

SDL_Window* window;
SDL_Renderer* renderer;

std::shared_ptr<Road> road;

std::shared_ptr<Player> player;
std::shared_ptr<Camera> mainCamera;

std::vector<std::shared_ptr<Segment>> segments;
int segmentsPassed = 0;

float iTime = 0.f;

// Testing-related
clock_t cycleTimeStart = 0.f, cycleTimeEnd = 0.f;

bool isRunning = true;

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

	mainCamera->update(DELTA_TIME);
	
}

void draw()
{
	rn::project(player->getDualVector(), mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	
	road->draw(renderer, *mainCamera);

	rn::dualVector floatingDot(rn::vector3f(0, 0, 1000));
	rn::project(floatingDot, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

	if (floatingDot.wV.z > player->getDualVector().wV.z) {
		SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
		gfxDrawBrenCircle(renderer, floatingDot.sV.x, floatingDot.sV.y, 10, true);
	}

	player->draw(renderer, *mainCamera);

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

	mainCamera = std::make_shared<Camera>(rn::vector3f(0, CAMERA_HEIGHT, 0), CAMERA_FOV);

	player = std::make_shared<Player>(rn::vector3f(0), *mainCamera);

	mainCamera->assignTarget(player);
}