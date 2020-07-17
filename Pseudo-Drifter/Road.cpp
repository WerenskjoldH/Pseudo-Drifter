#include "Road.h"
#include "Definitions.h"
#include "Camera.h"
#include "Ronin-Math-Library/vector.h"

Road::Road()
{
	Initialize();
}

Road::~Road()
{}

void Road::Initialize()
{
	// Initialize the starting road segments from closest to farthest
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

void Road::Update(float dt)
{}

void Road::Draw(SDL_Renderer* renderer, const Camera& camera)
{
	// Determine if we need to cull a road segment and place a new segment at the farthest distance
	// I'm still not certain if this should be a part of the draw step or not since it will also potentially affect game logic
	SegmentGeneration(camera.v);

	// Draw each segment
	for (int i = 0; i < SEGMENTS; i++)
	{
		DrawSegment(renderer, *roadSegments[i], camera);
	}
}


void Road::DrawSegment(SDL_Renderer* renderer, Segment& s, const Camera& camera)
{
		int rows = 0;

		// Take each vertex of the road segment and project its world coordinates to screen coordinates
		rn::project(s.bottomLeft, camera.v, camera.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
		rn::project(s.bottomRight, camera.v, camera.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
		rn::project(s.topLeft, camera.v, camera.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);
		rn::project(s.topRight, camera.v, camera.depth, WINDOW_WIDTH, WINDOW_HEIGHT, ROAD_WIDTH_DEFAULT);

		/*
			Distance-Based Draw Culling

			To-do: 
				- Camera-based culling
					+ This Should be reliant on camera properties, not internally stored variables, that will come with the Camera revamp
		*/

		if (s.bottomLeft.wV.z - camera.v.z > cullingDistance)
			return;

		if (s.topLeft.wV.z - camera.v.z - (camera.depth * camera.v.y) < 0)
			return;

		/*
			In the coming steps we are determining the boundaries of our screen-space road segment and creating a discrete range along the left and right slope of the road segment
			that we will draw a line between
		*/

		int bottomRow, topRow;
		bottomRow = s.bottomLeft.sV.y;
		if (bottomRow > WINDOW_HEIGHT)
			bottomRow = WINDOW_HEIGHT;

		rows = bottomRow - s.topLeft.sV.y;

		if (rows <= 0)
			return;

		float leftSlope = 1.f / ((s.topLeft.sV.y - s.bottomLeft.sV.y) / (s.topLeft.sV.x - s.bottomLeft.sV.x));
		float rightSlope = 1.f / ((s.topRight.sV.y - s.bottomRight.sV.y) / (s.topRight.sV.x - s.bottomRight.sV.x));

		// How many discrete points are there between the top vertexes and their bottom counterparts
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

			// Draw the x-axis components of the road ( outer colors, mid-line, etc. )
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

int Road::GetSegmentIndexFromDist(float dist)
{
	return (int)floor(int(dist / (float)SEGMENT_WIDTH)) % roadSegments.size();
}

void Road::SegmentGeneration(rn::vector3f camera)
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