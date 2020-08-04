#include "Engine.h"
#include "GfxHelper.h"
#include "ConsoleColorer.h"
#include "InputManager.h"
#include "Road.h"
#include "Player.h"
#include "Camera.h"
#include "Definitions.h"

#include "DrawableSystem.h"
#include "EntityFactory.h"

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

	systemManager = std::make_unique<SystemManager>(&entityManager);
	systemManager->AddSystem<DrawableSystem>();

	CreateBall(entityManager, rn::vector3f(20, 15, 650), rn::vector4f(100, 200, 120, 255));
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
	inputManager.BindKey(SDLK_i);
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

	systemManager->Update(DELTA_TIME);

	// This is for debugging purposes - to be removed
	if (inputManager.KeyPress(SDLK_i))
		entityManager.LogInfo();
}

void Engine::Draw()
{
	// Object Clipping
	// To-do: Clipping should occur here to prevent unnecessary draws

	road->Draw(renderer, *mainCamera);

	systemManager->Draw(renderer, *mainCamera);

	// Draws the player
	rn::project(player->dV, mainCamera->v, mainCamera->depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
	player->Draw(renderer, *mainCamera);

	// Swaps the buffer and clears the render target for the next draw step
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, DEFAULT_R, DEFAULT_G, DEFAULT_B, 255);
	SDL_RenderClear(renderer);
}