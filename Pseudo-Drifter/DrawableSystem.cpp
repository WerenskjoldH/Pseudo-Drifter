#include "DrawableSystem.h"
#include "GfxHelper.h"
#include "Definitions.h"
#include "Ronin-Math-Library/vector.h"

#include "Camera.h"
#include "PositionComponent.h"
#include "DrawableComponent.h"

DrawableSystem::DrawableSystem(EntityManager* iEntityManager) : System(iEntityManager)
{
}

void DrawableSystem::Draw(SDL_Renderer* renderer, const Camera& c)
{
	std::vector<std::shared_ptr<Entity>> entities =  entityManager->GetAllEntitiesWithComponent<DrawableComponent>();
	for (auto& e : entities)
	{
		std::shared_ptr<DrawableComponent> dc = entityManager->GetComponent<DrawableComponent>(e);
		std::shared_ptr<PositionComponent> pc = entityManager->GetComponent<PositionComponent>(e);

		if (!pc)
			continue;

		rn::project(pc->dV, c.v, c.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		GfxDrawBrenCircle(renderer, pc->dV.sV.x, pc->dV.sV.y, 20.f, true);
	}
}
