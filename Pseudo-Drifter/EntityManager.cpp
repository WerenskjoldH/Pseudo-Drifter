#include "EntityManager.h"
#include "ConsoleColorer.h"

EntityManager::EntityManager()
{
}

std::shared_ptr<Entity> EntityManager::AddEntity()
{
    Entity* e = new Entity(GenerateNewID());
    entities.emplace_back(e);
    return entities.at(entities.size()-1);
}

void EntityManager::RemoveEntity(std::shared_ptr<Entity> e)
{
    for (std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Component>>>>::iterator it = entityToComponents.begin(); it != entityToComponents.end(); it++)
    {
        std::string s = it->first;
        if (entityToComponents[s]->find(e) != entityToComponents[s]->end())
        {
            entityToComponents[s]->erase(e);
        }
    }

    for (auto it = entities.begin(); it != entities.end(); it++)
        if (*it == e) {
            entities.erase(it);
            return;
        }
}

std::shared_ptr<Component> EntityManager::GetComponent(const std::string s, std::shared_ptr<Entity> e)
{
    if (entityToComponents[s]->find(e) == entityToComponents[s]->end())
    {
        WRITE_CONSOLE_WARNING("ENTITY MANAGER", "WARNING", "Component not found");
    }

    return (*entityToComponents[s])[e];
}

std::vector<std::shared_ptr<Entity>> EntityManager::GetAllEntitiesWithComponent(std::string componentName)
{
    std::vector<std::shared_ptr<Entity>> entityList;

    std::shared_ptr<std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Component>>> entityComponents = entityToComponents[componentName];

    if (entityComponents)
    {
        for (auto& kvPair : (*entityComponents))
        {
            entityList.push_back(kvPair.first);
        }
    }

    return entityList;
}

void EntityManager::LogInfo()
{
    printf("--------Entity Manager Information--------\n");
    std::cout << "Entities: " << std::endl;

    if (entities.size() == 0)
        std::cout << "\tNone\n";

    for (auto& entityIt : entities)
    {
        std::cout << "\t" << entityIt->id << std::endl;
        std::cout << "\t- Entity " << entityIt->id << "\'s Components:" << std::endl;
        for (std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Component>>>>::iterator compIt = entityToComponents.begin(); compIt != entityToComponents.end(); compIt++)
        {
            std::string s = compIt->first;
            if (entityToComponents[s]->find(entityIt) != entityToComponents[s]->end())
            {
                std::cout << "\t\t" << (*entityToComponents[s])[entityIt]->GetComponentName() << std::endl;
            }
        }
    }

    printf("\n");

    for (std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Component>>>>::iterator compIt = entityToComponents.begin(); compIt != entityToComponents.end(); compIt++)
    {
        std::string s = compIt->first;
        std::cout << "Total " << s << "s: " << (*entityToComponents[s]).size() << std::endl;
    }

    printf("------------------------\n\n");
        
}

unsigned int EntityManager::GenerateNewID()
{
    // This is a very temporary fix and we really should be reusing ids that have been freed
    if (nextAvailableID == UINT32_MAX - 1)
    {
        WRITE_CONSOLE_WARNING("ENTITY MANAGER", "WARNING", "Hit max number of available IDs, returning to 0");
        nextAvailableID = 0;
    }

    return nextAvailableID++;
}
