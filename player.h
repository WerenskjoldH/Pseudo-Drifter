#pragma once
#include "gameObject.h"

class Camera;

class Player : public GameObject
{
private:
	float vertSpeed = 0;

	// Input is temporarily included in the player class
	bool keys[5]; // 0 - up, 1 - left, 2 - down, 3 - right, 4 - space

public:
	// Publically exposing this variable, dangerous, but okay for now
	rn::dualVector dV;

private:
	// These could likely be kept to being passed only as direct pointers might just cause tons of unnecessary coupling
	// std::shared_ptr<World> world
	// std::shared_ptr<Camera> camera;
	void input();

public:
	Player(const rn::vector3f startingPosition, float cameraDepth, float cameraHeight);
	~Player();

	void updateInputs(SDL_Event& e);
	void update(float dt) override;

	void draw(SDL_Renderer* renderer, rn::vector3f cameraPosition, float depth) override;

};