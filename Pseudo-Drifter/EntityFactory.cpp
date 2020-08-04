#include "EntityFactory.h"
//#include "EntityManager.h"
#include "PositionComponent.h"
#include "DrawableComponent.h"

std::shared_ptr<Entity> CreateBall(EntityManager& entityManager, rn::vector3f position, rn::vector4f color, int radius)
{
	std::shared_ptr<Entity> player = entityManager.AddEntity();
	entityManager.AddComponent<PositionComponent>(player, new PositionComponent(position));
	entityManager.AddComponent<DrawableComponent>(player, new DrawableComponent(color, radius));
	return player;
}
