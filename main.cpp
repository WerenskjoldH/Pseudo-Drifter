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

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400
#define WINDOW_PIXELS WINDOW_HEIGHT * WINDOW_WIDTH

#define SEGMENTS 50
#define SEGMENT_WIDTH 20
#define PERCENT_RUNNER 0.10f

#define DELTA_TIME 1/60.f
#define DEFAULT_R 140
#define DEFAULT_G 220
#define DEFAULT_B 220

#define CAMERA_HEIGHT 100
#define CAMERA_FOV 90

#define ROAD_WIDTH_DEFAULT 200.f

#define MAX_SPEED_VERTICAL 1.f
#define MAX_SPEED_HORIZONTAL 0.2f

enum ObjectShape
{
	CIRCLE,
	SQUARE
};

struct Vector2D {
	Vector2D(float x = 0, float y = 0, float z = 0) : x{ x }, y{ y }, z{ z }
	{	}

	float x, y, z;
	float sX = 0, sY = 0, sZ = 0;
};

static struct Player {
	// Add player.z to camera.z to get world position of player
	Vector2D v;
	float dist = 0;
	float horizSpeed = 0, horizAcceleration = 0, vertSpeed = 0, vertAcceleration = 0;
} player;

static struct Camera {
	Vector2D v;
	float depth;
} camera;

struct Segment {
	Segment(Vector2D tr, Vector2D tl, Vector2D br, Vector2D bl, float c, bool t) : topRight{ tr }, topLeft{tl}, bottomRight{ br }, bottomLeft{ bl }, color{ c }, type{ t }
	{	}

	// We will track the top-left points and the center of the top ( for simplicity sake )
	Vector2D topRight, bottomRight, topLeft, bottomLeft;
	float color = 0;
	bool type = 0;
};

SDL_Window* window;
SDL_Renderer* renderer;

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

// This is the bulk of how to render in this style
void project(Vector2D& v)
{
	// Transform the point to camera space
	// Essentially, we just find the distance from the point to the camera
	float cX = v.x - camera.v.x, cY = v.y - camera.v.y, cZ = v.z - camera.v.z;
	float scaling = camera.depth / cZ;

	// Transform the point from camera space to screen space x:[0 : 1] y:[0 : 1] from the top-left to the bottom-right and apply proper scaling
	v.sX = round((WINDOW_WIDTH / 2.f) + (scaling * cX * WINDOW_WIDTH / 2.f));
	v.sY = round((WINDOW_HEIGHT / 2.f) - (scaling * cY * WINDOW_HEIGHT / 2.f));
	v.sZ = round((scaling * ROAD_WIDTH_DEFAULT * (float)WINDOW_WIDTH / 2.f));
}

void drawSegment(Segment& s)
{
	int rows = 0;

	project(s.bottomLeft);
	project(s.bottomRight);
	project(s.topLeft);
	project(s.topRight);

	if (s.topLeft.z - camera.v.z - (camera.depth * camera.v.y) < 0)
		return;

	int bottomRow, topRow;
	bottomRow = s.bottomLeft.sY;
	if (bottomRow > WINDOW_HEIGHT)
		bottomRow = WINDOW_HEIGHT;

	rows = bottomRow - s.topLeft.sY;

	if (rows <= 0)
		return;

	float leftSlope = 1.f/((s.topLeft.sY - s.bottomLeft.sY)/(s.topLeft.sX - s.bottomLeft.sX));
	float rightSlope = 1.f/((s.topRight.sY - s.bottomRight.sY)/(s.topRight.sX - s.bottomRight.sX));

	int tLmBl = s.topLeft.sY - s.bottomLeft.sY;
	int tRmBr = s.topRight.sY - s.bottomRight.sY;

	for (int i = 0; i < rows; i++)
	{
		// Draw ground
		SDL_SetRenderDrawColor(renderer, 230, 200, 144, 255);
		SDL_RenderDrawLine(renderer, 0, s.topLeft.sY + i, WINDOW_WIDTH, s.topLeft.sY + i);

		// Calculate x value given y-position
		int x1 = (i + tLmBl) * leftSlope;
		int x2 = (i + tRmBr) * rightSlope;

		// Calculate inner points from x1 and x2
		int runnerLength = fabsf((s.bottomLeft.sX + x1) - (s.bottomRight.sX + x2)) * PERCENT_RUNNER;

		// Draw Road
		// Draw left edge, center, right edge

		if (s.type == 0)
			SDL_SetRenderDrawColor(renderer, 200, 100, 100, 255);
		else
			SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
		SDL_RenderDrawLine(renderer, s.bottomLeft.sX + x1, s.topLeft.sY + i, s.bottomLeft.sX + x1 + runnerLength, s.topRight.sY + i);
		SDL_RenderDrawLine(renderer, s.bottomRight.sX + x2, s.topLeft.sY + i, s.bottomRight.sX + x2 - runnerLength, s.topRight.sY + i);


		SDL_SetRenderDrawColor(renderer, s.color, s.color, s.color, 255);
		SDL_RenderDrawLine(renderer, s.bottomLeft.sX + x1 + runnerLength, s.topLeft.sY + i, s.bottomRight.sX + x2 - runnerLength, s.topRight.sY + i);
	}

}

inline int getSegmentIndexFromDist(float dist)
{
	return (int)floor(int(dist / (float)SEGMENT_WIDTH)) % segments.size();
}

int main(int args, char* argv[])
{
	// Create Window and Begin
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		std::cout << "Subsystems Initialized" << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Subsystems", "FATAL", "FAILED to initialize");
		isRunning = false;
	}

	window = SDL_CreateWindow("Driving Game - Hunter Werenskjold", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window)
		std::cout << "Window Created" << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Window", "FATAL", "Failed to be created");
		isRunning = false;
	}

	initialize();

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer)
		std::cout << "Renderer Created" << std::endl;
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

// We also handle clipping here
void segmentGeneration()
{
	/// Check the closest segment for clipping. If we should clip it, then we will remove the pointer from the vector alter the data accordingly and push it back on
	/// This should allow us to render only what we need, store only what we need, etc.
	if ((*segments[0]).topLeft.z - player.v.z < 0)
	{
		segments.erase(segments.begin());

		Uint8 color = 200;
		bool type = 0;
		if ((*segments[SEGMENTS - 1]).color == 200)
		{
			color = 220;
			type = 1;
		}

		// Generate and push back new segment onto vector
		// Preferably, we should probably just recycle the erased segment's data
		segments.push_back(std::make_shared<Segment>(
			Vector2D(ROAD_WIDTH_DEFAULT / 2.f, 0, (*segments[SEGMENTS - 1]).topLeft.z + SEGMENT_WIDTH),
			Vector2D(-1 * ROAD_WIDTH_DEFAULT / 2.f, 0, (*segments[SEGMENTS - 1]).topRight.z + SEGMENT_WIDTH),
			Vector2D(ROAD_WIDTH_DEFAULT / 2.f, 0, (*segments[SEGMENTS - 1]).topLeft.z),
			Vector2D(-1 * ROAD_WIDTH_DEFAULT / 2.f, 0, (*segments[SEGMENTS - 1]).topRight.z),
			color, type));
	}
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
		player.v.x -= 0.1f;

	if (keys[3] == 1)
		player.v.x += 0.1f;

	player.v.z += player.vertSpeed;

	camera.v.z = player.v.z - camera.depth * camera.v.y;
	camera.v.x = player.v.x;
}

void update()
{

	// Object Clipping
	checkClipping();
	segmentGeneration();

	// Input
	input();	
	
}

void draw()
{
	project(player.v);
	
	// Draw each segment
	for (int i = 0; i < SEGMENTS; i++)
	{
		drawSegment(*segments[i]);
	}

	Vector2D pd(0, 0, 1000);
	project(pd);

	if (pd.z > player.v.z) {
		SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
		gfxDrawBrenCircle(renderer, pd.sX, pd.sY, 10, true);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	gfxDrawBrenCircle(renderer, player.v.sX, player.v.sY, 20.f, true);


	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);
}

void initialize()
{
	camera.v.x = 0;
	camera.v.y = CAMERA_HEIGHT;
	camera.v.z = 0;
	camera.depth = 1.f / tanf((CAMERA_FOV / 2.f) * PI / 180.f);

	player.v.x = 0;
	player.v.y = 0;
	player.v.z = camera.depth*camera.v.y;

	// Going closest to farthest
	for (int i = 0; i <= SEGMENTS; i++)
	{
		Uint8 color = 200;
		bool type = 0;
		if (i % 2 == 0)
		{
			color = 220;
			 type = 1;
		}

		segments.push_back(std::make_shared<Segment>(
			Vector2D(ROAD_WIDTH_DEFAULT/2.f, 0, SEGMENT_WIDTH + i * SEGMENT_WIDTH),
			Vector2D(-1 * ROAD_WIDTH_DEFAULT / 2.f, 0, SEGMENT_WIDTH + i * SEGMENT_WIDTH),
			Vector2D(ROAD_WIDTH_DEFAULT/2.f, 0, i * SEGMENT_WIDTH),
			Vector2D(-1 * ROAD_WIDTH_DEFAULT/2.f, 0, i * SEGMENT_WIDTH),
			color, type));
	}
}