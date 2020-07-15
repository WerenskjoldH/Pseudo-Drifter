#ifndef PLAYER_H
#define PLAYER_H

/*
	Description: This file contains all declarations pertaining to the player class, the player class inherits from the abstract class GameObject

	To-do:
		* Speeds and all should be handled by a variable and NOT magic numbers
		* Decouple the camera reference in the constructor
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
	// Handles player input
	void movement();

public:
	Player(rn::vector3f startingPosition, const Camera& camera);
	~Player();

	/* Updates all inputs
		@param e The SDL_Event e passed from the game loop's inner event loop
	*/
	void updateInputs(SDL_Event& e);

	// @param dt Delta time
	void update(float dt) override;

	/*
		@param renderer Target renderer this object will draw to
		@param camera Camera this object will project to for rendering
	*/
	void draw(SDL_Renderer* renderer, const Camera& camera) override;

};

#endif