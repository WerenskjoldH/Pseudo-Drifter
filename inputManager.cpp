#include "inputManager.h"
#include "consoleColorer.h"
#include <iostream>

bool InputManager::validKey(SDL_Keycode k)
{
    return boundKeys.count(k);
}

void InputManager::bindKey(SDL_Keycode k)
{
    boundKeys.insert(std::make_pair(k, Key()));
}

bool InputManager::keyDown(SDL_Keycode k)
{
    if (!validKey(k)) 
    {
        WRITE_CONSOLE_WARNING("INPUT MANAGER", "MINOR", "Key down request for unbound key, please bind key before usage");
        return 0;
    }

    return boundKeys[k].state == 1;
}

bool InputManager::keyUp(SDL_Keycode k)
{
    if (!validKey(k))
    {
        WRITE_CONSOLE_WARNING("INPUT MANAGER", "MINOR", "Key up request for unbound key, please bind key before usage");
        return 0;
    }

    return boundKeys[k].state == 0;
}

bool InputManager::keyPress(SDL_Keycode k)
{
    if (!validKey(k))
    {
        WRITE_CONSOLE_WARNING("INPUT MANAGER", "MINOR", "Key press request for unbound key, please bind key before usage");
        return 0;
    }

    return boundKeys[k].state && !boundKeys[k].prevState;
}

bool InputManager::keyRelease(SDL_Keycode k)
{
    if (!validKey(k))
    {
        WRITE_CONSOLE_WARNING("INPUT MANAGER", "MINOR", "Key release request for unbound key, pelase bind key before usage");
        return 0;
    }

    return !boundKeys[k].state && boundKeys[k].prevState;
}

void InputManager::updateInput(SDL_Event& e)
{
    // If we don't check that it's a valid key we can accidentally bind it 
    if (!(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP || validKey(e.key.keysym.sym)))
        return;

    boundKeys[e.key.keysym.sym].state = e.type == SDL_KEYDOWN ? 1 : 0;
}

void InputManager::updatePrevInput()
{
    std::unordered_map<SDL_Keycode, Key>::iterator it = boundKeys.begin();
    while (it != boundKeys.end())
    {
        it->second.prevState = it->second.state;
        it++;
    }
}