/*
	Project Name: SNES-Style Driving Prototype
	Dev: Hunter Werenskjold
	Date: 10/1/19

	Description:
		This was a small project from 2019 that I have recently (July, 2020) decided to return to.
			The goal is to create a base for SNES-styled driving games, starting with basic rendering and forward-only progression
				As far as design goes, I'm being a bit loose, so expect rash sudden changes, sorry
				If this project gets a large enough code-base going, I will start working on experimental branches before major revisions are made

	Acknowledgements to Jake Gordon's fantastic article on creating an old-school pseudo-3d racing game in Javascript https://codeincomplete.com/articles/javascript-racer/
*/

#include "Engine.h"
#include "EntityManager.h"
#include "DrawableComponent.h"
#include "PositionComponent.h"

int main(int args, char* argv[])
{
	Engine e;
	return e.Begin();
}