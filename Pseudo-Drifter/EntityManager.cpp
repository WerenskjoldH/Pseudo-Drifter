#include "EntityManager.h"
#include "ConsoleColorer.h"

EntityManager::EntityManager()
{
}

const Entity& EntityManager::AddObject()
{
    Entity e(GenerateNewID());
    entities.push_back(e);
    return e;
}

unsigned int EntityManager::GenerateNewID()
{
    // This is a very temporary fix and we really should be reusing ids that have been freed
    if (curID == UINT32_MAX - 1)
    {
        WRITE_CONSOLE_WARNING("ENTITY MANAGER", "WARNING", "Hit max number of available IDs, returning to 0");
        curID = 0;
    }

    return curID++;
}
