#pragma once

#include <SDL/SDL.h>
#include "vector.h"

class GameObject
{
protected:
	rn::dualVector positionDV;
public:
	virtual ~GameObject();

	rn::dualVector& getDualVector();

	virtual void update(float dt) = 0;
	virtual void draw(SDL_Renderer* renderer, rn::vector3f cameraPosition, float depth) = 0;
};