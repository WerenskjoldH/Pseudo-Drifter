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

#include <unordered_map>
#include <memory>
#include <vector>

#include "ConsoleColorer.h"

#include "Entity.h"
#include "Component.h"

class EntityManager
{
private:
	// Added for simplicity & readability
	typedef std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Component>> entityComponentMap;

private:
	// This is a mapping from component name to an entity <-> component map
	// Generic component name -> (entity -> entity's component data)
	std::unordered_map<std::string, std::shared_ptr<entityComponentMap>> entityToComponents;

	std::vector<std::shared_ptr<Entity>> entities;

	int nextAvailableID = 0;

public:
	EntityManager();

	std::shared_ptr<Entity> AddEntity();
	void RemoveEntity(std::shared_ptr<Entity> e);

	template <class T>
	std::shared_ptr<T> AddComponent(std::shared_ptr<Entity> e, T* c)
	{
		const char* componentName = typeid(T).name();

		auto entityComponents = entityToComponents[componentName];
		if (!entityComponents)
		{
			entityComponents = std::make_shared<entityComponentMap>();
			entityToComponents[componentName] = entityComponents;
		}
		
		std::shared_ptr<T> sharedC = std::make_shared<T>(*c);

		(*entityComponents)[e] = sharedC;

		return sharedC;
	}

	template <class T>
	std::shared_ptr<T> GetComponent(std::shared_ptr<Entity> e)
	{
		const char* componentName = typeid(T).name();

		if (entityToComponents[componentName]->find(e) == entityToComponents[componentName]->end())
		{
			WRITE_CONSOLE_WARNING("ENTITY MANAGER", "WARNING", "Component not found");
		}

 		return std::static_pointer_cast<T>((*entityToComponents[componentName])[e]);
	}

	template <class T>
	std::vector<std::shared_ptr<Entity>> GetAllEntitiesWithComponent()
	{
		const char* componentName = typeid(T).name();

		std::vector<std::shared_ptr<Entity>> entityList;

		auto entityComponents = entityToComponents[componentName];

		if (entityComponents)
		{
			for (auto& kvPair : (*entityComponents))
			{
				entityList.push_back(kvPair.first);
			}
		}

		return entityList;
	}

	void LogInfo();


private:
	unsigned int GenerateNewID() 
	{     
		// This is a temporary fix and we really should be reusing ids that have been freed
		if (nextAvailableID == UINT32_MAX - 1)
		{
			WRITE_CONSOLE_WARNING("ENTITY MANAGER", "WARNING", "Hit max number of available IDs, returning to 0");
			nextAvailableID = 0;
		}

		return nextAvailableID++;
	}
		;
};

#endif
