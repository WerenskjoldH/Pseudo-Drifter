#pragma once
#include "gameObject.h"

class Camera;

class Player : public GameObject
{
private:
	// These could likely be kept to being passed only as direct pointers might just cause tons of unnecessary coupling
	// std::shared_ptr<World> world
	// std::shared_ptr<Camera> camera;
	void input();

public:
	Player(const rn::vector3f startingPosition);
	~Player();

	void update(float dt) override;

	void draw(SDL_Renderer* renderer, rn::vector3f cameraPosition, float depth) override;

};