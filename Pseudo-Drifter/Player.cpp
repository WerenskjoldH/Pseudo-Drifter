#include "Player.h"
#include "GfxHelper.h"
#include "Definitions.h"
#include "Camera.h"
#include "InputManager.h"

static float FloatClamp(float v, float min, float max)
{
	return (v >= min && v <= max) ? v : (v < min) ? min : max;
}

Player::Player(rn::vector3f startingPosition, const Camera& camera) : GameObject()
{
	dV.wV = startingPosition;
	dV.wV.z = camera.depth * camera.v.y;
}

Player::~Player()
{
}

void Player::Update(float dt)
{
	Movement();
}

void Player::Movement()
{
	if (G_INPUT->KeyDown(SDLK_w))
	{
		velocity.z += 0.001f;
	}

	if (G_INPUT->KeyDown(SDLK_s))
	{
		velocity.z -= 0.001f;
	}

	velocity.z = FloatClamp(velocity.z, -1 * MAX_SPEED_VERTICAL, MAX_SPEED_VERTICAL);

	if (G_INPUT->KeyDown(SDLK_a) == 1)
		velocity.x -= 0.1f;

	if (G_INPUT->KeyDown(SDLK_d))
		velocity.x += 0.1f;

	rn::vector3f opposingForce;
	opposingForce += VEHICLE_DRAG_FORCE * velocity * velocity.magnitude();
	opposingForce += VEHICLE_ROLLING_RESISTANCE * velocity;

	dV.wV += velocity + opposingForce;

	velocity.x = 0;
}

void Player::Draw(SDL_Renderer* renderer, const Camera& c)
{
	// Currently we just draw the player as a good ole-fashioned circle
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	GfxDrawBrenCircle(renderer, dV.sV.x, dV.sV.y, 20.f, true);
}