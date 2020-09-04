#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

/*
	Description: This file contains all declarations pertaining to the position component class
	
	This is a purely data-oriented component and does not have a corresponding system
*/

#include "Ronin-Math-Library/vector.h";

#include "Component.h"

class PositionComponent : public Component
{
public:
	rn::dualVector dV;

	PositionComponent(rn::vector3f worldPosition);
};

#endif
