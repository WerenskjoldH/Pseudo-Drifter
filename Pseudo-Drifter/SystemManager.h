#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

/*
	Description: This file contains all declarations and definitions pertaining to the system manager class

	This class manages all systems

	To-Do:
		* Refactoring
		* Storing entities with the systems in a vector and using entity signatures is probably a good idea, this current implementation is sloppy
*/

#include <memory>
#include <unordered_map>
#include <SDL.h>

#include "ConsoleColorer.h"
#include "System.h"

class SystemManager
{
private:
	std::unordered_map<const char*, std::shared_ptr<System>> systems;
	EntityManager* entityManager = nullptr;

public:
	SystemManager(EntityManager* entityManager) : entityManager{ entityManager } {};
	
	template <class T>
	std::shared_ptr<T> AddSystem()
	{
		const char* systemName = typeid(T).name();

		
		if (systems.find(systemName) != systems.end())
		{
			WRITE_CONSOLE_WARNING("SYSTEM MANAGER", "WARNING", "Attempted to add system a second time");
			return nullptr;
		}
		auto system = std::make_shared<T>();
		system->AssignEntityManager(entityManager);
		systems[systemName] = std::static_pointer_cast<System>(system);
		return system;
	}

	template <class T>
	std::shared_ptr<System> GetSystem()
	{
		const char* systemName = typeid(T).name();

		if (systems.find(systemName) == systems.end())
		{
			WRITE_CONSOLE_WARNING("SYSTEM MANAGER", "WARNING", "Attempted to get system that hasn't been added");
			return nullptr;
		}

		return systems[systemName];
	}

	void Update(float dt)
	{
		for (auto& kvPair : systems)
		{
			kvPair.second->Update(dt);
		}
	}

	void Draw(SDL_Renderer* renderer, const Camera& camera)
	{
		for (auto& kvPair : systems)
		{
			kvPair.second->Draw(renderer, camera);
		}
	}

};

#endif