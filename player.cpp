#include "player.h"
#include "gfxHelper.h"
#include "definitions.h"

Player::Player(rn::vector3f startingPosition, float cameraDepth, float cameraHeight) : GameObject()
{
	positionDV.wV = startingPosition;
	dV.wV.z = cameraDepth * cameraHeight;
}

Player::~Player()
{
}

void Player::update(float dt)
{
	input();
}

void Player::draw(SDL_Renderer* renderer, rn::vector3f cameraPosition, float depth)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	gfxDrawBrenCircle(renderer, dV.sV.x, dV.sV.y, 20.f, true);
}

void Player::input()
{
	if (keys[0] == 1 && vertSpeed < MAX_SPEED_VERTICAL)
	{
		vertSpeed += 0.001f;
	}

	if (keys[2] == 1 && vertSpeed > -1 * MAX_SPEED_VERTICAL)
	{
		vertSpeed -= 0.001f;
	}

	vertSpeed -= 0.0001f;

	if (vertSpeed < 0)
		vertSpeed = 0;

	if (keys[1] == 1)
		dV.wV.x -= 0.1f;

	if (keys[3] == 1)
		dV.wV.x += 0.1f;

	dV.wV.z += vertSpeed;
}

void Player::updateInputs(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
		if (e.key.keysym.sym == SDLK_w)
			keys[0] = 1;
		else if (e.key.keysym.sym == SDLK_a)
			keys[1] = 1;
		else if (e.key.keysym.sym == SDLK_s)
			keys[2] = 1;
		else if (e.key.keysym.sym == SDLK_d)
			keys[3] = 1;
		else if (e.key.keysym.sym == SDLK_SPACE)
			keys[4] = 1;

	if (e.type == SDL_KEYUP)
		if (e.key.keysym.sym == SDLK_w)
			keys[0] = 0;
		else if (e.key.keysym.sym == SDLK_a)
			keys[1] = 0;
		else if (e.key.keysym.sym == SDLK_s)
			keys[2] = 0;
		else if (e.key.keysym.sym == SDLK_d)
			keys[3] = 0;
		else if (e.key.keysym.sym == SDLK_SPACE)
			keys[4] = 0;
}
