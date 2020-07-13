#include "road.h"
#include "definitions.h"

Road::Road()
{
	// Going closest to farthest
	for (int i = 0; i <= SEGMENTS; i++)
	{
		Uint8 color = 200;
		bool type = 0;
		if (i % 2 == 0)
		{
			color = 220;
			type = 1;
		}

		roadSegments.push_back(std::make_shared<Segment>(
			rn::vector3f(ROAD_WIDTH_DEFAULT / 2.f, 0, SEGMENT_WIDTH + i * SEGMENT_WIDTH),
			rn::vector3f(-1 * ROAD_WIDTH_DEFAULT / 2.f, 0, SEGMENT_WIDTH + i * SEGMENT_WIDTH),
			rn::vector3f(ROAD_WIDTH_DEFAULT / 2.f, 0, i * SEGMENT_WIDTH),
			rn::vector3f(-1 * ROAD_WIDTH_DEFAULT / 2.f, 0, i * SEGMENT_WIDTH),
			color, type));
	}
}

Road::~Road()
{
}

void Road::update(float dt)
{
	//segmentGeneration()
}

void Road::draw(SDL_Renderer* renderer, rn::vector3f camera, float depth)
{
	segmentGeneration(camera);

	// Draw each segment
	for (int i = 0; i < SEGMENTS; i++)
	{
		drawSegment(renderer, *roadSegments[i], camera, depth);
	}
}


void Road::drawSegment(SDL_Renderer* renderer, Segment& s, const rn::vector3f& camera, float depth)
{
		int rows = 0;

		rn::project(s.bottomLeft, camera, depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
		rn::project(s.bottomRight, camera, depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
		rn::project(s.topLeft, camera, depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
		rn::project(s.topRight, camera, depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

		if (s.topLeft.wV.z - camera.z - (depth * camera.y) < 0)
			return;

		int bottomRow, topRow;
		bottomRow = s.bottomLeft.sV.y;
		if (bottomRow > WINDOW_HEIGHT)
			bottomRow = WINDOW_HEIGHT;

		rows = bottomRow - s.topLeft.sV.y;

		if (rows <= 0)
			return;

		float leftSlope = 1.f / ((s.topLeft.sV.y - s.bottomLeft.sV.y) / (s.topLeft.sV.x - s.bottomLeft.sV.x));
		float rightSlope = 1.f / ((s.topRight.sV.y - s.bottomRight.sV.y) / (s.topRight.sV.x - s.bottomRight.sV.x));

		int tLmBl = s.topLeft.sV.y - s.bottomLeft.sV.y;
		int tRmBr = s.topRight.sV.y - s.bottomRight.sV.y;

		for (int i = 0; i < rows; i++)
		{
			// Draw ground
			SDL_SetRenderDrawColor(renderer, 230, 200, 144, 255);
			SDL_RenderDrawLine(renderer, 0, s.topLeft.sV.y + i, WINDOW_WIDTH, s.topLeft.sV.y + i);

			// Calculate x value given y-position
			int x1 = (i + tLmBl) * leftSlope;
			int x2 = (i + tRmBr) * rightSlope;

			// Calculate inner points from x1 and x2
			int runnerLength = fabsf((s.bottomLeft.sV.x + x1) - (s.bottomRight.sV.x + x2)) * PERCENT_RUNNER;

			// Draw Road
			// Draw left edge, center, right edge

			if (s.type == 0)
				SDL_SetRenderDrawColor(renderer, 200, 100, 100, 255);
			else
				SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
			SDL_RenderDrawLine(renderer, s.bottomLeft.sV.x + x1, s.topLeft.sV.y + i, s.bottomLeft.sV.x + x1 + runnerLength, s.topRight.sV.y + i);
			SDL_RenderDrawLine(renderer, s.bottomRight.sV.x + x2, s.topLeft.sV.y + i, s.bottomRight.sV.x + x2 - runnerLength, s.topRight.sV.y + i);


			SDL_SetRenderDrawColor(renderer, s.color, s.color, s.color, 255);
			SDL_RenderDrawLine(renderer, s.bottomLeft.sV.x + x1 + runnerLength, s.topLeft.sV.y + i, s.bottomRight.sV.x + x2 - runnerLength, s.topRight.sV.y + i);
		}
}

int Road::getSegmentIndexFromDist(float dist)
{
	return (int)floor(int(dist / (float)SEGMENT_WIDTH)) % roadSegments.size();
}

void Road::segmentGeneration(rn::vector3f camera)
{
	/// Check the closest segment for clipping. If we should clip it, then we will remove the pointer from the vector alter the data accordingly and push it back on
	/// This should allow us to render only what we need, store only what we need, etc.
	if ((*roadSegments[0]).topLeft.wV.z - camera.z < 0)
	{
		roadSegments.erase(roadSegments.begin());

		Uint8 color = 200;
		bool type = 0;
		if ((*roadSegments[SEGMENTS - 1]).color == 200)
		{
			color = 220;
			type = 1;
		}

		// Generate and push back new segment onto vector
		// Preferably, we should probably just recycle the erased segment's data
		roadSegments.push_back(std::make_shared<Segment>(
			rn::vector3f(ROAD_WIDTH_DEFAULT / 2.f, 0, (*roadSegments[SEGMENTS - 1]).topLeft.wV.z + SEGMENT_WIDTH),
			rn::vector3f(-1 * ROAD_WIDTH_DEFAULT / 2.f, 0, (*roadSegments[SEGMENTS - 1]).topRight.wV.z + SEGMENT_WIDTH),
			rn::vector3f(ROAD_WIDTH_DEFAULT / 2.f, 0, (*roadSegments[SEGMENTS - 1]).topLeft.wV.z),
			rn::vector3f(-1 * ROAD_WIDTH_DEFAULT / 2.f, 0, (*roadSegments[SEGMENTS - 1]).topRight.wV.z),
			color, type));
	}
}