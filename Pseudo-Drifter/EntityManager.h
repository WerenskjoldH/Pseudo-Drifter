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
	std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Component>>>> entityToComponents;

	std::vector<std::shared_ptr<Entity>> entities;

	int nextAvailableID = 0;

public:
	EntityManager();

	std::shared_ptr<Entity> AddEntity();
	void RemoveEntity(std::shared_ptr<Entity> e);

	template <class T>
	void AddComponent(std::shared_ptr<Entity> e, T* c)
	{
		std::shared_ptr<std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Component>>> entityComponents = entityToComponents[c->GetComponentName()];
		if (!entityComponents)
		{
			entityComponents = std::make_shared<std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Component>>>();
			entityToComponents[c->GetComponentName()] = entityComponents;
		}
		(*entityComponents)[e] = std::make_shared<T>(*c);
	}

	std::shared_ptr<Component> GetComponent(const std::string s, std::shared_ptr<Entity> e);

	std::vector<std::shared_ptr<Entity>> GetAllEntitiesWithComponent(std::string componentName);

	void LogInfo();


private:
	unsigned int GenerateNewID();
};

#endif
