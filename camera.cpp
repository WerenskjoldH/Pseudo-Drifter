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

void Camera::AssignTarget(std::shared_ptr<GameObject> t)
{
	target = t;
}

void Camera::Update(float dt)
{
	// Without a target we should not attempt to update the camera's position
	if (target == nullptr)
	{
		std::cout << INLINE_COLOR_FONT("Camera::WARNING::Attempting to update camera without a target, with the current system this is dangerous", FONT_YELLOW) << std::endl;
		return;
	}

	v.z = target->GetDualVector().wV.z - depth * v.y;
	v.x = target->GetDualVector().wV.x;
}
