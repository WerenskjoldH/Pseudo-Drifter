#ifndef CAMERA_H
#define CAMERA_H

/*
	Description: This file contains all declarations pertaining to the camera class.
	
	The Camera class is work-in-progress and instances of the class currently just track behind an object moving without rotation on the x and z axis, point in the positive z-axis.

	To-do:
		* Each camera instance should have its own render target and be used by all drawable objects during the draw step to draw to its render target.
		* Easing functions for moving the camera
		* Additional (proper) support for target independent cameras
			- I.e. track cameras, mirrors, etc.
		* Rotation
		* Different tracking modes for camera instances with targets
*/

#include "vector.h"

// Being able to support multiple cameras in the scene and deciding which one to render with is a powerful thing
//	Mirrors are a great example of this

// The Camera class SHOULD hold its own renderer ( or a reference ) and handle all drawing to allow easy additions of post-fx
//	However, this will likely come at a later time

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

#endif