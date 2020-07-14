#pragma once

#include "vector.h"

// Being able to support multiple cameras in the scene and deciding which one to render with is a powerful thing
//	Mirrors are a great example of this

class GameObject;

// For now cameras are more of a data-only structure
//	This is likely to expand with time however
class Camera
{
public:
	rn::vector3f v;

	// Distance behind object
	float depth;

	std::shared_ptr<GameObject> target;

private:

public:
	Camera(rn::vector3f position, float cameraDepth);
	~Camera();

	void assignTarget(std::shared_ptr<GameObject> t);

	void update(float dt);

};