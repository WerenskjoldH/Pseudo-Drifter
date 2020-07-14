#ifndef PLAYER_H
#define PLAYER_H

#include "gameObject.h"

class Camera;

class Player : public GameObject
{
private:
	float vertSpeed = 0;

	// Input is temporarily included in the player class
	bool keys[5]; // 0 - up, 1 - left, 2 - down, 3 - right, 4 - space

private:
	// These could likely be kept to being passed only as direct pointers might just cause tons of unnecessary coupling
	// std::shared_ptr<World> world
	// std::shared_ptr<Camera> camera;
	void input();

public:
	Player(rn::vector3f startingPosition, const Camera& camera);
	~Player();

	void updateInputs(SDL_Event& e);
	void update(float dt) override;

	void draw(SDL_Renderer* renderer, const Camera& camera) override;

};

#endif