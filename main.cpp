#include <iostream>
#include <vector>

#include "gfxHelper.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define WINDOW_PIXELS WINDOW_HEIGHT * WINDOW_WIDTH

#define DRAW_DISTANCE 250
#define SEGMENTS DRAW_DISTANCE
#define SEGMENT_WIDTH 100

#define DELTA_TIME 1/60.f
#define DEFAULT_R 140
#define DEFAULT_G 220
#define DEFAULT_B 220

#define CAMERA_HEIGHT 100
#define CAMERA_FOV 45

#define ROAD_WIDTH_DEFAULT 200.f

#define MAX_SPEED_VERTICAL 4.f
#define MAX_SPEED_HORIZONTAL 0.2f

enum ObjectShape
{
	CIRCLE,
	SQUARE
};

// Stores quite a bit more than a slightly beefed up primitive should
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
	Segment(Vector2D tr, Vector2D tl, Vector2D br, Vector2D bl, float c) : topRight{ tr }, topLeft{tl}, bottomRight{ br }, bottomLeft{ bl }, color{ c }
	{	}

	// We will track the top-left points and the center of the top ( for simplicity sake )
	Vector2D topRight, bottomRight, topLeft, bottomLeft;
	float color = 0;
};

SDL_Window* window;
SDL_Renderer* renderer;

std::vector<Segment*> segments;

bool keys[5]; // 0 - up, 1 - left, 2 - down, 3 - right, 4 - space

float iTime = 0.f;

bool isRunning = true;

void updateInputs(SDL_Event& e);
void update();
void draw();

void initialize();

void project(Vector2D& v)
{
	// Transform the point to camera space
	// Essentially, we just find the distance from the point to the camera
	float cX = v.x - camera.v.x, cY = v.y - camera.v.y, cZ = v.z - camera.v.z;
	float scaling = camera.depth / cZ;

	// Transform the point from camera space x:[-1 : 1] and y:[-1 : 1] to screen space x:[0 : 1] y:[0 : 1] from the top-left to the bottom-right and apply proper scaling
	v.sX = round((WINDOW_WIDTH / 2.f) + (scaling * cX * WINDOW_WIDTH / 2.f));
	v.sY = round((WINDOW_HEIGHT / 2.f) - (scaling * cY * WINDOW_HEIGHT / 2.f));
	v.sZ = round((scaling * ROAD_WIDTH_DEFAULT * WINDOW_WIDTH / 2.f));
}

void drawSegment(Segment& s)
{
	int rows = 0;

	project(s.bottomLeft);
	project(s.bottomRight);
	project(s.topLeft);
	project(s.topRight);

	if (s.topLeft.z - camera.v.z < 0)
		return;

	int bottomRow, topRow;
	bottomRow = s.bottomLeft.sY;
	if (bottomRow > WINDOW_HEIGHT)
		bottomRow = WINDOW_HEIGHT;

	rows = bottomRow - s.topLeft.sY;

	if (rows <= 0)
		return;

	float leftSlope = (s.topLeft.sY - s.bottomLeft.sY)/(s.topLeft.sX - s.bottomLeft.sX);
	float rightSlope = (s.topRight.sY - s.bottomRight.sY)/(s.topRight.sX - s.bottomRight.sX);

	/*
	For each row r; iterate from p1 to p2
		draw road w/ color and width calculated from topWidth + i * interpolationValue
	*/
	for (int i = 0; i < rows; i++)
	{
		// Draw ground
		SDL_SetRenderDrawColor(renderer, 230, 200, 144, 255);
		SDL_RenderDrawLine(renderer, 0, s.topLeft.sY + i, WINDOW_WIDTH, s.topLeft.sY + i);

		// Calculate slope-intercept form for x
		int x1 = ((s.topLeft.sY + i) - s.bottomLeft.sY) / leftSlope;
		int x2 = ((s.topRight.sY + i) - s.bottomRight.sY) / rightSlope;

		// Draw Road
		SDL_SetRenderDrawColor(renderer, s.color, s.color, s.color, 255);
		SDL_RenderDrawLine(renderer, s.bottomLeft.sX + x1, s.topLeft.sY + i, s.bottomRight.sX + x2, s.topRight.sY + i);
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
		std::cout << "ERROR::FATAL::Subsystems FAILED to initalize" << std::endl;
		isRunning = false;
	}

	window = SDL_CreateWindow("Driving Game - Hunter Werenskjold", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window)
		std::cout << "Window Created" << std::endl;
	else
	{
		std::cout << "ERROR::FATAL::Window FAILED to be created" << std::endl;
		isRunning = false;
	}

	initialize();

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer)
		std::cout << "Renderer Created" << std::endl;
	else
	{
		std::cout << "ERROR::FATAL::Subsystems FAILED to be created" << std::endl;
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

void update()
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

void draw()
{
	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);

	project(player.v);

	for (int i = 1; i < SEGMENTS; i++)
	{
		drawSegment(*segments[i]);
	}

	Vector2D pd(0, 0, 1000);
	project(pd);

	SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
	gfxDrawBrenCircle(renderer, pd.sX, pd.sY, 10.f, true);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	gfxDrawBrenCircle(renderer, player.v.sX, player.v.sY, 20.f, true);


	SDL_RenderPresent(renderer);
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
	for (int i = 0; i <= DRAW_DISTANCE; i++)
	{
		segments.push_back(new Segment(
			Vector2D(ROAD_WIDTH_DEFAULT/2.f, 0, SEGMENT_WIDTH + i * SEGMENT_WIDTH),
			Vector2D(-1 * ROAD_WIDTH_DEFAULT / 2.f, 0, SEGMENT_WIDTH + i * SEGMENT_WIDTH),
			Vector2D(ROAD_WIDTH_DEFAULT/2.f, 0, i * SEGMENT_WIDTH),
			Vector2D(-1 * ROAD_WIDTH_DEFAULT/2.f, 0, i * SEGMENT_WIDTH),
			244 * (i % 2)));
	}
}