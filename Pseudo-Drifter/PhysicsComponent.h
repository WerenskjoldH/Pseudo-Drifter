#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

/*
	Description: This file contains all declarations pertaining to the physics component class

	To-Do:
		* This should maybe contain helper methods for applying forces, handling collision forces, etc.
*/

#include "Ronin-Math-Library/vector.h"

#include "Component.h"
#include "InputManager.h"

class PhysicsComponent : public Component
{
public:
	rn::vector3f velocity;
	rn::vector3f acceleration;

	PhysicsComponent();
};

#endif