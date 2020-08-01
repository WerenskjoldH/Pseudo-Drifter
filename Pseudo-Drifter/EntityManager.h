#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

/*
	Description: This file contains all declarations pertaining to the entity manager class

	This class handles the creation, handling, and destruction of entities as well as managing all the systems that will act on the entities' components

	To-Do:
		* More Efficient GenerateNewID(), it's unlikely to reach that many entities and overflow, but it sure would be embarrassing to crash because of it
*/

#include "Entity.h"
#include "Component.h"

#include <unordered_map>
#include <vector>

class EntityManager
{
private:
	std::unordered_map<std::string, std::unordered_map<unsigned int, Component>> entityToComponents;

	std::vector<Entity> entities;

	int curID = 0;

public:
	EntityManager();

	const Entity& AddObject();

private:
	unsigned int GenerateNewID();
};

#endif
