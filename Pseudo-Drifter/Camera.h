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

#include "Ronin-Math-Library/vector.h"

class GameObject;
class PositionComponent;
class EntityManager;
class Entity;

class Camera
{
public:
	// Depth of the camera behind the near-plane, understand this as being related to FOV
	float depth;

	rn::vector3f v;

	std::shared_ptr<PositionComponent> target;

	/*
		@param position World space position of the camera
		@param cameraFOV Field-of-view of the camera given in degrees
	*/
	Camera(rn::vector3f position, float cameraFOV);
	~Camera();

	//@param dt Delta time
	void Update(float dt);

	/* Assigns the target of the camera
		@param positionComp Positional component being assigned as the camera target
	*/
	void AssignTarget(EntityManager* entityManager, std::shared_ptr<Entity> e);
};

#endif