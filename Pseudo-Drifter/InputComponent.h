#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

/*
	Description: This file contains all declarations pertaining to the input component class
*/

#include "Component.h"
#include "InputManager.h"
#include <memory>

class InputComponent : public Component
{
public:
	std::shared_ptr<InputManager> inputManager;

	InputComponent(std::shared_ptr<InputManager> _inputManager);
};

#endif