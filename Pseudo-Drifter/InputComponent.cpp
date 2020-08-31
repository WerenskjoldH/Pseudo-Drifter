#include "InputComponent.h"

InputComponent::InputComponent(std::shared_ptr<InputManager> _inputManager) : Component("InputComponent"), inputManager{ _inputManager }
{
}
