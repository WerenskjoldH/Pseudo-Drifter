#ifndef SYSTEM_H
#define SYSTEM_H

/*
	Description: This file contains all declarations pertaining to the System class

	This class is quite barebones serving only as a parent class
*/

#include "EntityManager.h"
#include <SDL.h>

class Camera;

class System
{
protected:
	EntityManager* entityManager;

public:
	System() {};
	~System() {};

	void AssignEntityManager(EntityManager* iEntityManager) {
		entityManager = iEntityManager;
	};

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Renderer* renderer, const Camera& c) {};

};

#endif