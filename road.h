#ifndef ROAD_H
#define ROAD_H

// That's some ambiguity right there, thank goodness for namespaces!
#include <vector>
#include "vector.h"

#include <SDL/SDL.h>

class Camera;

struct Segment {
	// We will track the top-left points and the center of the top ( for simplicity sake )
	rn::dualVector topRight, bottomRight, topLeft, bottomLeft;
	float color = 0;
	bool type = 0;

	Segment(rn::vector3f tr, rn::vector3f tl, rn::vector3f br, rn::vector3f bl, float c, bool t) : topRight{ tr }, topLeft{ tl }, bottomRight{ br }, bottomLeft{ bl }, color{ c }, type{ t }
	{	}
};

class Road
{
private:
	std::vector<std::shared_ptr<Segment>> roadSegments;

public:

private:
	void drawSegment(SDL_Renderer* renderer, Segment& s, const Camera& camera);
	void segmentGeneration(rn::vector3f camera);
	int getSegmentIndexFromDist(float dist);

public:
	Road();
	~Road();

	void update(float dt);
	void draw(SDL_Renderer* renderer, const Camera& c);
};

#endif