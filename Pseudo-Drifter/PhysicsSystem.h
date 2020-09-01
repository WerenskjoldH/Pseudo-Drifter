#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

/*
	Description: This file contains all declarations pertaining to the Physics System class

	To-Do:
		* This is kept basic for now, handling collisions and world forces are still TBD
*/

#include "System.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem();

	void Update(float dt) override;
};


#endif