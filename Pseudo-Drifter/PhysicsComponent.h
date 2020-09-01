#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

/*
	Description: This file contains all declarations pertaining to the physics component class

	To-Do:
		* This should maybe contain helper methods for applying forces, handling collision forces, etc.
*/

#include "Component.h"
#include "InputManager.h"
#include "Ronin-Math-Library/vector.h"

class PhysicsComponent : public Component
{
public:
	rn::vector3f velocity;
	rn::vector3f acceleration;

	PhysicsComponent() : Component("PhysicsComponent") { velocity = 0; acceleration = 0; }
};

#endif