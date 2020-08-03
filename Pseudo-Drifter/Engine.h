#ifndef ENGINE_H
#define ENGINE_H

/*
	Description: This file contains all declarations pertaining to the Engine class

	This is the entry point for our game to be built on. Currently, most of our scene/game code is being run directly inside the starting Engine instance,
		and that is bad. However, this will slowly be corrected and expanded as features get added and ironed out. So don't worry.

	To-do:
		* Scene Swapping
		* Abstracting Functionality Out, i.e. Road should not exist in here & We need a game object manager to handle our world and object to object interactions
		* Occlusion Culling 
*/

#include "Definitions.h"
#include "InputManager.h"

#include "EntityManager.h"
#include "SystemManager.h"

#include <SDL.h>
#include <memory>
#include <vector>

class Road;
class Player;
class InputManager;
class Segment;
class Camera;

class Engine
{

public:
	Engine();
	~Engine();
	int Begin();

	// @return Returns whether the game is still running or not
	bool CheckRunning();

private:
	bool isRunning = true;
	
	float totalTimeSinceStart = 0;

	std::shared_ptr<Road> road;
	std::shared_ptr<Player> player;
	std::shared_ptr<Camera> mainCamera;

	EntityManager entityManager;
	std::unique_ptr<SystemManager> systemManager;

	InputManager inputManager;

	SDL_Window* window;
	SDL_Renderer* renderer;

	void Initialize();

	void InitSDL();
	void InitInput();
	void InitWorld();

	void Update();
	/* Handles Rendering
		The rendering order goes as follows:
			Road
			Scene objects ( farthest to closest ) [ Note: Ordering is required ]
			Player
			Foreground Elements
			U.I.
	*/
	void Draw();
};


#endif