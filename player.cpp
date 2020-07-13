#include "player.h"

Player::Player(rn::vector3f startingPosition) : GameObject()
{
	positionDV.wV = startingPosition;
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
}

void Player::input()
{
	
}