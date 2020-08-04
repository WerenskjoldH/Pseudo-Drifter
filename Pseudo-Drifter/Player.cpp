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
	Movement(dt);
}

void Player::Movement(float dt)
{
	rn::vector3f carHeading(0);

	if (G_INPUT->KeyDown(SDLK_w))
		carHeading.z = 1;

	//if (G_INPUT->KeyDown(SDLK_s))
	//	carHeading.z = -1;

	if (G_INPUT->KeyDown(SDLK_a))
		carHeading.x = -1;

	if (G_INPUT->KeyDown(SDLK_d))
		carHeading.x = 1;

	carHeading.normalize();

 	velocity += (VEHICLE_ENGINE_FORCE * carHeading) * dt;
	velocity = carHeading * FloatClamp(velocity.magnitude(), 0, MAX_SPEED_VERTICAL);

	dV.wV += velocity * dt;
}

void Player::Draw(SDL_Renderer* renderer, const Camera& c)
{
	// Currently we just draw the player as a good ole-fashioned circle
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	GfxDrawBrenCircle(renderer, dV.sV.x, dV.sV.y, 20.f, true);
}