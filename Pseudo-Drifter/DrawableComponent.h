#ifndef DRAWABLE_COMPONENT_H
#define DRAWABLE_COMPONENT_H

/*
	Description: This file contains all declarations pertaining to the drawable component class
	
	Do note that for now this class will only draw circles
*/

#include "Component.h"
#include "Ronin-Math-Library/vector.h";

class DrawableComponent : public Component
{
public:
	rn::vector4f color;

	DrawableComponent(rn::vector4f iColor);
};

#endif
