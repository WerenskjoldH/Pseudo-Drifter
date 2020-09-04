#include "EntityManager.h"

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
    for (auto it = entityToComponents.begin(); it != entityToComponents.end(); it++)
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
        for (auto compIt = entityToComponents.begin(); compIt != entityToComponents.end(); compIt++)
        {
            std::string s = compIt->first;
            if (entityToComponents[s]->find(entityIt) != entityToComponents[s]->end())
            {
                std::cout << "\t\t" << (*entityToComponents[s])[entityIt]->GetComponentName() << std::endl;
            }
        }
    }

    printf("\n");

    for (auto compIt = entityToComponents.begin(); compIt != entityToComponents.end(); compIt++)
    {
        std::string s = compIt->first;
        std::cout << "Total " << s.substr(6) << "s: " << (*entityToComponents[s]).size() << std::endl;
    }

    printf("------------------------\n\n");
        
}