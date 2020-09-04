#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

/*
	Description: This file contains all declarations for the GameObject virtual class

	This class is ONLY to be inherited from, the pure functions should ensure that

	To-do:
		* Right now, nothing
*/

#include <SDL.h>

#include "Ronin-Math-Library/vector.h"

class Camera;

class GameObject
{
public:
	// Dual vector used for storing world space and projected screen-space of the game object
	// Getters/Setters proved pointless so far for this
	rn::dualVector dV;

	virtual ~GameObject();
	// @param dt Delta time
	virtual void Update(float dt) = 0;

	/*
		@param renderer Target renderer this object will draw to
		@param camera Camera this object will project to for rendering
	*/
	virtual void Draw(SDL_Renderer* renderer, const Camera& camera) = 0;
};
#endif