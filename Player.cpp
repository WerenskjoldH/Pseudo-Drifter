#include "Player.h"
#include "GfxHelper.h"
#include "Definitions.h"
#include "Camera.h"
#include "InputManager.h"

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

void Player::Draw(SDL_Renderer* renderer, const Camera& c)
{
	// Currently we just draw the player as a good ole-fashioned circle
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	GfxDrawBrenCircle(renderer, dV.sV.x, dV.sV.y, 20.f, true);
}

void Player::Movement()
{
	if (G_INPUT->KeyDown(SDLK_w) && vertSpeed < MAX_SPEED_VERTICAL)
	{
		vertSpeed += 0.001f;
	}

	if (G_INPUT->KeyDown(SDLK_s) && vertSpeed > -1 * MAX_SPEED_VERTICAL)
	{
		vertSpeed -= 0.001f;
	}

	vertSpeed -= 0.0001f;

	if (vertSpeed < 0)
		vertSpeed = 0;

	if (G_INPUT->KeyDown(SDLK_a) == 1)
		dV.wV.x -= 0.1f;

	if (G_INPUT->KeyDown(SDLK_d))
		dV.wV.x += 0.1f;

	dV.wV.z += vertSpeed;
}