#include "EntityFactory.h"

#include "PositionComponent.h"
#include "DrawableComponent.h"
#include "PhysicsComponent.h"
#include "Camera.h"

std::shared_ptr<Entity> CreateBall(EntityManager& entityManager, rn::vector3f position, rn::vector4f color, int radius)
{
	std::shared_ptr<Entity> ball = entityManager.AddEntity();
	entityManager.AddComponent<PositionComponent>(ball, new PositionComponent(position));
	entityManager.AddComponent<DrawableComponent>(ball, new DrawableComponent(color, radius));
	return ball;
}

std::shared_ptr<Entity> CreatePlayer(EntityManager& entityManager, Camera& cam, rn::vector3f position, rn::vector4f color, int radius)
{
	std::shared_ptr<Entity> player = entityManager.AddEntity();
	entityManager.AddComponent<PositionComponent>(player, new PositionComponent(position))->dV.wV.z = cam.depth * cam.v.y;

	//entityManager.GetComponent<PositionComponent>(player)->dV.wV.z = cam.depth * cam.v.y;

	entityManager.AddComponent<DrawableComponent>(player, new DrawableComponent(color, radius));
	entityManager.AddComponent<PhysicsComponent>(player, new PhysicsComponent());
	return player;
}
