#include "Engine.h"
#include "GfxHelper.h"
#include "ConsoleColorer.h"
#include "InputManager.h"
#include "Road.h"
#include "Player.h"
#include "Camera.h"
#include "Definitions.h"

// Global variable(s)
InputManager* G_INPUT = nullptr;

Engine::Engine()
{
	Initialize();
}

Engine::~Engine()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Engine::Initialize()
{
	InitSDL();
	InitInput();
	InitWorld();
}

void Engine::InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		std::cout << INLINE_COLOR_FONT("Subsystems Initialized", FONT_GREEN) << std::endl;
	else
	{
		WRITE_CONSOLE_ERROR("Subsystems", "FATAL", "FAILED to initialize");
		isRunning = false;
	}

	// Create window instance
	window = SDL_CreateWindow("Pseudo-Driver - Hunter Werenskjold", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
}

void Engine::InitInput()
{
	inputManager.BindKey(SDLK_w);
	inputManager.BindKey(SDLK_a);
	inputManager.BindKey(SDLK_s);
	inputManager.BindKey(SDLK_d);
	inputManager.BindKey(SDLK_SPACE);

	G_INPUT = &inputManager;
}

void Engine::InitWorld()
{
	road = std::make_shared<Road>();

	mainCamera = std::make_shared<Camera>(rn::vector3f(0, CAMERA_HEIGHT, 0), CAMERA_FOV);

	// The camera parameter of the Player class constructor is actually not that necessary and should be decoupled in the future
	player = std::make_shared<Player>(rn::vector3f(0), *mainCamera);

	// Assigns the target for the main camera to follow
	mainCamera->AssignTarget(player);
}

bool Engine::CheckRunning()
{
	return false;
}

int Engine::Begin()
{
	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);

	// Do note that SDL_GetTicks() returns milliseconds, not centiseconds
	float frameStartTime = SDL_GetTicks() * .01f;
	float timeAccumulator = 0.f;

	SDL_Event e;
	// Game loop	
	while (isRunning)
	{
		const float currentTime = SDL_GetTicks() * .01f;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			{
				isRunning = false;
			}

			G_INPUT->UpdateInput(e);
		}

		/*
			Deterministic Timesteps
			To-Do:
				- Split Logic & "Physics"/Fixed Updates
				- Interpolated Drawing(?)
		*/

		timeAccumulator += (currentTime - frameStartTime);
		frameStartTime = currentTime;

		// If there is some heavy slow-downs due to hefty calculations or the likes, this will prevent the accumulator growing too large to maintain a good frame-rate
		timeAccumulator = (timeAccumulator > MAX_DELTA_TIME) ? MAX_DELTA_TIME : timeAccumulator;

		while (timeAccumulator >= DELTA_TIME) 
		{
			Update();
			timeAccumulator -= DELTA_TIME;
			totalTimeSinceStart += DELTA_TIME;
		}

		Draw();

		G_INPUT->UpdatePrevInput();
	}

	return 0;
}

void Engine::Update()
{
	road->Update(DELTA_TIME);

	player->Update(DELTA_TIME);

	mainCamera->Update(DELTA_TIME);
}

void Engine::Draw()
{
	// Object Clipping
	// To-do: Clipping should occur here to prevent unnecessary draws

	road->Draw(renderer, *mainCamera);

	// This is currently rough testing code for creating dots that sit on the track
	rn::dualVector floatingDotCenter(rn::vector3f(0, 0, 1000));
	rn::dualVector floatingDotTop(rn::vector3f(0, 10, 1000));
	rn::project(floatingDotCenter, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	rn::project(floatingDotTop, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

	rn::dualVector floatingDotTwoCenter(rn::vector3f(10, 20, 2000));
	rn::dualVector floatingDotTwoTop(rn::vector3f(10, 30, 2000));
	rn::project(floatingDotTwoCenter, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	rn::project(floatingDotTwoTop, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

	if (floatingDotCenter.wV.z > player->dV.wV.z) {
		SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
		GfxDrawBrenCircle(renderer, floatingDotCenter.sV.x, floatingDotCenter.sV.y, (floatingDotTop.sV - floatingDotCenter.sV).magnitude(), true);
	}

	if (floatingDotTwoCenter.wV.z > player->dV.wV.z) {
		SDL_SetRenderDrawColor(renderer, 144, 255, 144, 255);
		GfxDrawBrenCircle(renderer, floatingDotTwoCenter.sV.x, floatingDotTwoCenter.sV.y, (floatingDotTwoTop.sV - floatingDotTwoCenter.sV).magnitude(), true);
	}

	// Draws the player
	rn::project(player->dV, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	player->Draw(renderer, *mainCamera);

	// Swaps the buffer and clears the render target for the next draw step
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);
}