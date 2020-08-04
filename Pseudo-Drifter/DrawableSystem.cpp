#include "DrawableSystem.h"
#include "GfxHelper.h"
#include "Definitions.h"
#include "Ronin-Math-Library/vector.h"

#include "Camera.h"
#include "PositionComponent.h"
#include "DrawableComponent.h"

DrawableSystem::DrawableSystem()
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

		// This is quite temporary until the ECS is fully implemented
		if (pc->dV.wV.z > c.v.z)
		{
			rn::dualVector circleTop(pc->dV.wV);
			circleTop.wV.y += dc->radius;
			rn::project(pc->dV, c.v, c.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
			rn::project(circleTop, c.v, c.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

			// This will temporarily handle clipping
			if (circleTop.sV.y < WINDOW_HEIGHT) 
			{
				SDL_SetRenderDrawColor(renderer, dc->color.r(), dc->color.g(), dc->color.b(), dc->color.a());
				GfxDrawBrenCircle(renderer, pc->dV.sV.x, pc->dV.sV.y, (circleTop.sV - pc->dV.sV).magnitude(), true);
			}
		}
	}
}
