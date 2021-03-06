#ifndef DRAWABLE_SYSTEM_H
#define DRAWABLE_SYSTEM_H

/*
	Description: This file contains all declarations pertaining to the Drawable System class

	To-Do:
		* This will be expanded to draw sprites instead of just circles
*/

#include "System.h"

class DrawableSystem : public System
{
public:
	DrawableSystem();

	void Draw(SDL_Renderer* renderer, const Camera& c) override;
};


#endif