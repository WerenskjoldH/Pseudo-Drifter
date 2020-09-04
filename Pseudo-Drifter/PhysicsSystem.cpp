#include "PhysicsSystem.h"

#include "Definitions.h"

#include "PhysicsComponent.h"
#include "PositionComponent.h"

static float FloatClamp(float v, float min, float max)
{
	return (v >= min && v <= max) ? v : (v < min) ? min : max;
}

PhysicsSystem::PhysicsSystem()
{
}

void PhysicsSystem::Update(float dt)
{
	std::vector<std::shared_ptr<Entity>> entities = entityManager->GetAllEntitiesWithComponent<PhysicsComponent>();
	for (auto& e : entities)
	{
		std::shared_ptr<PhysicsComponent> physicsComp = entityManager->GetComponent<PhysicsComponent>(e);
		std::shared_ptr<PositionComponent> positionComp = entityManager->GetComponent<PositionComponent>(e);

		if (!positionComp || !physicsComp)
			continue;

		/*
			Everything below this is temporary and expected to change!

			These should be broken into a:
				- Vehicle Controller ( input forces that modify physics component )
				- Player Controller ( act on vehicle to give player steering, this will also allow us to hook in AI drivers easily )

			We should decouple the physics system such that it only integrates the physics component of each entity based on its velocity, acceleration, etc.
		*/

		rn::vector3f carHeading(0);

		if (G_INPUT->KeyDown(SDLK_w))
			carHeading.z = 1;

		//if (G_INPUT->KeyDown(SDLK_s))
		//	carHeading.z = -1;

		if (G_INPUT->KeyDown(SDLK_a))
			carHeading.x = -1;

		if (G_INPUT->KeyDown(SDLK_d))
			carHeading.x = 1;

		carHeading.normalize();

		physicsComp->velocity += (VEHICLE_ENGINE_FORCE * carHeading) * dt;
		physicsComp->velocity = carHeading * FloatClamp(physicsComp->velocity.magnitude(), 0, MAX_SPEED_VERTICAL);

		positionComp->dV.wV += physicsComp->velocity * dt;
		
	}
}
