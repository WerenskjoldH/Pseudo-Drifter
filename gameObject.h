#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

/*
	Description: This file contains all declarations for the GameObject virtual class

	This class is ONLY to be inherited from, the pure functions should ensure that

	To-do:
		* That getDualVector() function is looking mighty unsafe, it was implemented to ease the refactoring process from the earliest version of the engine to what it is now
*/

#include <SDL/SDL.h>
#include "vector.h"

class Camera;

class GameObject
{
protected:
	// Dual vector used for storing world space and projected screen-space of a point/vertex
	rn::dualVector dV;
public:
	virtual ~GameObject();

	// @return Gets a direct reference to the game objects dual vector
	rn::dualVector& getDualVector();

	// @param dt Delta time
	virtual void update(float dt) = 0;

	/*
		@param renderer Target renderer this object will draw to
		@param camera Camera this object will project to for rendering
	*/
	virtual void draw(SDL_Renderer* renderer, const Camera& camera) = 0;
};
#endif