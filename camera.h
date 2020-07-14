#ifndef CAMERA_H
#define CAMERA_H

/*
	Description: This file contains all declarations pertaining to the camera class.
	
	The Camera class is work-in-progress and instances of the class currently just track behind an object moving without rotation on the x and z axis, point in the positive z-axis.

	NOTE: All camera instances MUST be given a target, if not they will report a WARNING every frame

	To-do:
		* Each camera instance should have its own render target and be used by all drawable objects during the draw step to draw to its render target.
		* The camera should handle culling and ordering of draws, but first we should simply look into a function to describe if an objects screen space coordinates are culled or not. This is all run on one thread of the CPU and is expensive. That also beckons the need for multi-threading at some point...
		* Easing functions for moving the camera
		* Additional (proper) support for target independent cameras
			- I.e. tracking cameras, mirrors, etc.
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

	// Depth of the camera behind the near-plane, understand this as being related to FOV
	float depth;

	std::shared_ptr<GameObject> target;

private:

public:
	/*
		@param position World space position of the camera
		@param cameraFOV Field-of-view of the camera given in degrees
	*/
	Camera(rn::vector3f position, float cameraFOV);
	~Camera();

	/* Assigns the target of the camera
		@param t Game object being assigned as the camera target
	*/
	void assignTarget(std::shared_ptr<GameObject> t);

	//@param dt Delta time
	void update(float dt);

};

#endif