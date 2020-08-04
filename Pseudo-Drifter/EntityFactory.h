#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

/*
	Description: A collection of functions for creating common entities

	This is temporary
*/

#include "Ronin-Math-Library/vector.h"
#include "EntityManager.h"

std::shared_ptr<Entity> CreateBall(EntityManager& entityManager, rn::vector3f position, rn::vector4f color = rn::vector4f(0), int radius = 10.0f);
#endif