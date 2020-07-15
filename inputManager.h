#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

/*
	Description: This file contains all declarations pertaining to the InputManager class and its dependencies.

	The input manager requires that you bind keys before usage, from there it will manage those key inputs and all updates to them. To get information from a bound key, 
		you should use the keyDown(), keyUp(), ... functions.

	To-do:
		* Mouse support
		* This is a tad rigid, i.e. what if we want controller support?
*/

#include <SDL/SDL.h>
#include <unordered_map>

struct Key
{
	bool state = 0;
	bool prevState = 0;
};

class InputManager
{
private:
	// Since unordered maps don't allow duplicate keys, we can check existance in near linear time!
	std::unordered_map<SDL_Keycode, Key> boundKeys;
	
	bool validKey(SDL_Keycode k);

public:
	void bindKey(SDL_Keycode k);

	bool keyDown(SDL_Keycode k);
	bool keyUp(SDL_Keycode k);
	bool keyPress(SDL_Keycode k);
	bool keyRelease(SDL_Keycode k);

	// This must go inside the game loop's inner event loop
	void updateInput(SDL_Event& e);
	// Update at end of frame, this is used for deltas
	void updatePrevInput();
};

#endif