#pragma once

#include "vector.h"

struct Segment {
	// We will track the top-left points and the center of the top ( for simplicity sake )
	rn::vector3f topRight, bottomRight, topLeft, bottomLeft;
	float color = 0;
	bool type = 0;

	Segment(rn::vector3f tr, rn::vector3f tl, rn::vector3f br, rn::vector3f bl, float c, bool t) : topRight{ tr }, topLeft{ tl }, bottomRight{ br }, bottomLeft{ bl }, color{ c }, type{ t }
	{	}
};

class Road
{
private:
	Road()
	{

	}

public:
};