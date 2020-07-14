#include "camera.h"
#include "gameObject.h"
#include "consoleColorer.h"

Camera::Camera(rn::vector3f position, float cameraFOV)
{
	v = position;
	
	depth = 1.f / tanf((cameraFOV / 2.f) * 3.14159265358979323 / 180.f);
}

Camera::~Camera()
{
}

void Camera::assignTarget(std::shared_ptr<GameObject> t)
{
	target = t;
}

void Camera::update(float dt)
{
	if (target == nullptr)
	{
		std::cout << INLINE_COLOR_FONT("Camera::WARNING::Attempting to update camera without a target, with the current system this is dangerous", FONT_YELLOW) << std::endl;
		return;
	}

	v.z = target->getDualVector().wV.z - depth * v.y;
	v.x = target->getDualVector().wV.x;
}
