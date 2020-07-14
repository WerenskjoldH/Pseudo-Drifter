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
	rn::dualVector dV;
public:
	virtual ~GameObject();

	rn::dualVector& getDualVector();

	virtual void update(float dt) = 0;
	virtual void draw(SDL_Renderer* renderer, const Camera& camera) = 0;
};
#endif