/*
	Project Name: SNES-Style Driving Prototype
	Dev: Hunter Werenskjold
	Date: 10/1/19

	Description:
		This was a small project from 2019 that I have recently (July, 2020) decided to return to.
			The goal is to create a base for SNES-styled driving games, starting with basic rendering and forward-only progression
				As far as design goes, I'm being a bit loose, so expect rash sudden changes, sorry
				If this project gets a large enough code-base going, I will start working on experimental branches before major revisions are made

	Acknowledgements to Jake Gordon's fantastic article on creating an old-school pseudo-3d racing game in Javascript https://codeincomplete.com/articles/javascript-racer/
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

void initialize()
{
	/*
		Handle SDL Initilizations and Setup
	*/
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		std::cout << INLINE_COLOR_FONT("Subsystems Initialized", FONT_GREEN) << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Subsystems", "FATAL", "FAILED to initialize");
		isRunning = false;
	}

	// Create window instance
	window = SDL_CreateWindow("Driving Game - Hunter Werenskjold", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window)
		std::cout << INLINE_COLOR_FONT("Window Created", FONT_GREEN) << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Window", "FATAL", "Failed to be created");
		isRunning = false;
	}

	// Create renderer instance
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
		std::cout << INLINE_COLOR_FONT("Renderer Created", FONT_GREEN) << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Renderer", "FATAL", "Failed to be created");
		isRunning = false;
	}

	/*
		Initializes all required objects to start the game/load starting scene
	*/

	road = std::make_shared<Road>();

	mainCamera = std::make_shared<Camera>(rn::vector3f(0, CAMERA_HEIGHT, 0), CAMERA_FOV);

	// The camera parameter of the Player class constructor is actually not that necessary and should be decoupled in the future
	player = std::make_shared<Player>(rn::vector3f(0), *mainCamera);

	// Assigns the target for the main camera to follow
	mainCamera->assignTarget(player);
}

int main(int args, char* argv[])
{
	initialize();

	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);

	SDL_Event e;
	// Game loop	
	while (isRunning)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
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

// All logic pertaining to per-frame updates are called from here
//	To-do: I would like to add a staggered update as well to handle things that do not need to occur every frame
void update()
{
	// Object culling
	// To-do: Culling of unnecessary/unused/destroyed/etc. objects should occur here

	road->update(DELTA_TIME);

	player->update(DELTA_TIME);

	mainCamera->update(DELTA_TIME);
	
}

// All logic pertaining to drawing is contained or called from here
void draw()
{
	// Object Clipping
	// To-do: Clipping should occur here to prevent unnecessary draws
	
	/*
		The rendering order goes as follows:
			Road
			Scene objects ( farthest to closest ) [ Note: Ordering is required ]
			Player
			Foreground Elements
			U.I.
	*/

	road->draw(renderer, *mainCamera);

	// This is currently rough testing code for creating dots that sit on the track
	rn::dualVector floatingDotCenter(rn::vector3f(0, 0, 1000));
	rn::dualVector floatingDotTop(rn::vector3f(0, 10, 1000));
	rn::project(floatingDotCenter, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	rn::project(floatingDotTop, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

	rn::dualVector floatingDotTwoCenter(rn::vector3f(10, 20, 2000));
	rn::dualVector floatingDotTwoTop(rn::vector3f(10, 30, 2000));
	rn::project(floatingDotTwoCenter, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	rn::project(floatingDotTwoTop, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

	if (floatingDotCenter.wV.z > player->getDualVector().wV.z) {
		SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
		gfxDrawBrenCircle(renderer, floatingDotCenter.sV.x, floatingDotCenter.sV.y, (floatingDotTop.sV - floatingDotCenter.sV).magnitude(), true);
	}

	if (floatingDotTwoCenter.wV.z > player->getDualVector().wV.z) {
		SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
		gfxDrawBrenCircle(renderer, floatingDotTwoCenter.sV.x, floatingDotTwoCenter.sV.y, (floatingDotTwoTop.sV - floatingDotTwoCenter.sV).magnitude(), true);
	}

	// Draws the player
	rn::project(player->getDualVector(), mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	player->draw(renderer, *mainCamera);

	// Swaps the buffer and clears the render target for the next draw step
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);
}