#ifndef PLAYER_H
#define PLAYER_H

/*
	Description: This file contains all declarations pertaining to the player class, the player class inherits from the abstract class GameObject

	To-do:
		* Refactor input handling into its own class that is passed to the player instance
		* Allow player to assign which camera is the main view, this would allow the player to swap between front, overhead, and rear views
		* Add a vehicle super-class that the player -and other vehicle dependent- class can use for vehicle handling information and the likes
		* Create player sprite support with turning sprites
*/

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