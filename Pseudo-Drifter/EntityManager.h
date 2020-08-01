#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

/*
	Description: This file contains all declarations pertaining to the entity manager class

	This class handles the creation, handling, and destruction of entities as well as managing all the systems that will act on the entities' components

	To-Do:
		* Refactor this HARD
		* More Efficient GenerateNewID(), it's unlikely to reach that many entities and overflow, but it sure would be embarrassing to crash because of it
		* shared_ptr is being abused 
		* Is a vector really best given how often we may remove objects? The reduced cache misses might still mean yes. I want to explore this
		* Memory Pooling on Maps
*/

#include "Entity.h"
#include "Component.h"

#include <unordered_map>
#include <memory>
#include <vector>

class EntityManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<std::unordered_map<unsigned int, std::shared_ptr<Component>>>> entityToComponents;

	std::vector<std::shared_ptr<Entity>> entities;

	int curID = 0;

public:
	EntityManager();

	std::shared_ptr<Entity> AddEntity();
	void RemoveEntity(std::shared_ptr<Entity> e);

	void AddComponent(std::shared_ptr<Entity> e, Component* c);
	std::shared_ptr<Component> GetComponent(const std::string s, std::shared_ptr<Entity> e);

	void LogInfo();


private:
	unsigned int GenerateNewID();
};

#endif
