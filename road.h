#ifndef ROAD_H
#define ROAD_H

/*
	Description: This file contains all declarations pertaining to the road class and its dependencies.

	The road, and subsiquently the segment struct, is used for drawing the road onto the given render target. 
		It is quite simple, but more than enough for right now.

	To-do:
		* Need a function w/ a boolean return type to test collisions with the road
		* Optimize road segments to re-use culled segments, as well perhaps optimize distance culling
		* Ability to add more randomized elements to give more character to the tracks
		* Road styles w/ various properties
			- I.e. if we want a dirt road perhaps we have more friction, faster steering, but more difficult -slippery- controls
*/

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