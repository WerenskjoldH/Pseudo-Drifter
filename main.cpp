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

#include "definitions.h"

enum ObjectShape
{
	CIRCLE,
	SQUARE
};

static struct Player {
	// Add player.z to camera.z to get world position of player
	rn::dualVector dV;
	float dist = 0;
	float horizSpeed = 0, horizAcceleration = 0, vertSpeed = 0, vertAcceleration = 0;
} player;

static struct Camera {
	rn::vector3f v;
	float depth;
} camera;

SDL_Window* window;
SDL_Renderer* renderer;

std::shared_ptr<Road> road;

std::vector<std::shared_ptr<Segment>> segments;
int segmentsPassed = 0;

bool keys[5]; // 0 - up, 1 - left, 2 - down, 3 - right, 4 - space

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

	initialize();

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer)
		std::cout << INLINE_COLOR_FONT("Renderer Created", FONT_GREEN) << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Renderer", "FATAL", "Failed to be created");
		isRunning = false;
	}

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
			
			updateInputs(e);

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

void updateInputs(SDL_Event& e)
{

	if (e.type == SDL_KEYDOWN)
		if (e.key.keysym.sym == SDLK_w)
			keys[0] = 1;
		else if (e.key.keysym.sym == SDLK_a)
			keys[1] = 1;
		else if (e.key.keysym.sym == SDLK_s)
			keys[2] = 1;
		else if (e.key.keysym.sym == SDLK_d)
			keys[3] = 1;
		else if (e.key.keysym.sym == SDLK_SPACE)
			keys[4] = 1;

	if (e.type == SDL_KEYUP)
		if (e.key.keysym.sym == SDLK_w)
			keys[0] = 0;
		else if (e.key.keysym.sym == SDLK_a)
			keys[1] = 0;
		else if (e.key.keysym.sym == SDLK_s)
			keys[2] = 0;
		else if (e.key.keysym.sym == SDLK_d)
			keys[3] = 0;
		else if (e.key.keysym.sym == SDLK_SPACE)
			keys[4] = 0;
}

// Check clipping of objects and segments
void checkClipping()
{
	// Clip Objects
}

void input()
{
	if (keys[0] == 1 && player.vertSpeed < MAX_SPEED_VERTICAL)
	{
		player.vertSpeed += 0.001f;
	}

	if (keys[2] == 1 && player.vertSpeed > -1 * MAX_SPEED_VERTICAL)
	{
		player.vertSpeed -= 0.001f;
	}

	player.vertSpeed -= 0.0001f;

	if (player.vertSpeed < 0)
		player.vertSpeed = 0;

	if (keys[1] == 1)
		player.dV.wV.x -= 0.1f;

	if (keys[3] == 1)
		player.dV.wV.x += 0.1f;

	player.dV.wV.z += player.vertSpeed;

	camera.v.z = player.dV.wV.z - camera.depth * camera.v.y;
	camera.v.x = player.dV.wV.x;
}

void update()
{

	// Object Clipping
	checkClipping();

	road->update(DELTA_TIME);

	// Input
	input();	
	
}

void draw()
{
	rn::project(player.dV, camera.v, camera.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	
	road->draw(renderer, camera.v, camera.depth);

	rn::dualVector pd(rn::vector3f(0, 0, 1000));
	rn::project(pd, camera.v, camera.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

	if (pd.wV.z > player.dV.wV.z) {
		SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
		gfxDrawBrenCircle(renderer, pd.sV.x, pd.sV.y, 10, true);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	gfxDrawBrenCircle(renderer, player.dV.sV.x, player.dV.sV.y, 20.f, true);


	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);
}

void initialize()
{
	road = std::make_shared<Road>();

	camera.v.x = 0;
	camera.v.y = CAMERA_HEIGHT;
	camera.v.z = 0;
	camera.depth = 1.f / tanf((CAMERA_FOV / 2.f) * PI / 180.f);

	player.dV.wV.x = 0;
	player.dV.wV.y = 0;
	player.dV.wV.z = camera.depth*camera.v.y;
}