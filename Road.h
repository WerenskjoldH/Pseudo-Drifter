#ifndef ROAD_H
#define ROAD_H

/*
	Description: This file contains all declarations pertaining to the road class and its dependencies.

	The road, and subsiquently the segment struct, is used for drawing the road onto the given render target. 
		It is quite simple, but more than enough for right now.

	To-do:
		* Road should handle how many segments should be drawn and not hard-coded by a pre-processor definition
		* Need a function w/ a boolean return type to test collisions with the road
		* Optimize road segments to re-use culled segments, as well perhaps optimize distance culling
		* Ability to add more randomized elements to give more character to the tracks
		* Road styles w/ various properties
			- I.e. if we want a dirt road perhaps we have more friction, faster steering, but more difficult -slippery- controls

	Acknowledgements: The handling of drawing road segments (and the idea of this project) is based off of the javascript SNES-styled driving game linked below
	Link: https://codeincomplete.com/articles/javascript-racer/
*/

#include <vector>

#include <SDL/SDL.h>

#include "vector.h"

class Camera;

// Segment of road, these are individually rendered to create a continuous road
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
	// Contains all active road segments
	std::vector<std::shared_ptr<Segment>> roadSegments;

public:

private:
	/*	Draws a given segment
		@param renderer Renderer the segment is drawn to
		@param s Segment to be drawn
		@param camera Camera to be projected to
	*/
	void DrawSegment(SDL_Renderer* renderer, Segment& s, const Camera& camera);
	
	/* Generates new segments when passed segments are culled
		@param camera Camera that is rendered to
	*/
	void SegmentGeneration(rn::vector3f camera);

	/*
		Gets segment index in vector, from the road distance from start
		@param z-axis distance
	*/
	int GetSegmentIndexFromDist(float dist);

public:
	Road();
	~Road();

	// @param dt Delta time
	void Update(float dt);

	/*
		@param renderer Target renderer this object will draw to
		@param camera Camera this object will project to for rendering
	*/
	void Draw(SDL_Renderer* renderer, const Camera& c);
};

#endif