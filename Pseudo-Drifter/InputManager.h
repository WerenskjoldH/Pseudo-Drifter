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

#include <SDL.h>
#include <unordered_map>

struct Key
{
	bool state = 0;
	bool prevState = 0;
};

class InputManager
{
public:
	InputManager();
	~InputManager();

	// This must go inside the game loop's inner event loop
	void UpdateInput(SDL_Event& e);
	// Update at end of frame, this is used for deltas
	void UpdatePrevInput();

	void BindKey(SDL_Keycode k);

	bool KeyUp(SDL_Keycode k);
	bool KeyDown(SDL_Keycode k);
	bool KeyPress(SDL_Keycode k);
	bool KeyRelease(SDL_Keycode k);

private:
	// Since unordered maps don't allow duplicate keys, we can check existance in near linear time!
	std::unordered_map<SDL_Keycode, Key> boundKeys;

	bool ValidKey(SDL_Keycode k);
};

#endif