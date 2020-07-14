#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

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