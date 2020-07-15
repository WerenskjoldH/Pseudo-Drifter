#include "inputManager.h"
#include "consoleColorer.h"
#include <iostream>

bool InputManager::ValidKey(SDL_Keycode k)
{
    return boundKeys.count(k);
}

void InputManager::BindKey(SDL_Keycode k)
{
    boundKeys.insert(std::make_pair(k, Key()));
}

bool InputManager::KeyDown(SDL_Keycode k)
{
    if (!ValidKey(k)) 
    {
        WRITE_CONSOLE_WARNING("INPUT MANAGER", "MINOR", "Key down request for unbound key, please bind key before usage");
        return 0;
    }

    return boundKeys[k].state == 1;
}

bool InputManager::KeyUp(SDL_Keycode k)
{
    if (!ValidKey(k))
    {
        WRITE_CONSOLE_WARNING("INPUT MANAGER", "MINOR", "Key up request for unbound key, please bind key before usage");
        return 0;
    }

    return boundKeys[k].state == 0;
}

bool InputManager::KeyPress(SDL_Keycode k)
{
    if (!ValidKey(k))
    {
        WRITE_CONSOLE_WARNING("INPUT MANAGER", "MINOR", "Key press request for unbound key, please bind key before usage");
        return 0;
    }

    return boundKeys[k].state && !boundKeys[k].prevState;
}

bool InputManager::KeyRelease(SDL_Keycode k)
{
    if (!ValidKey(k))
    {
        WRITE_CONSOLE_WARNING("INPUT MANAGER", "MINOR", "Key release request for unbound key, pelase bind key before usage");
        return 0;
    }

    return !boundKeys[k].state && boundKeys[k].prevState;
}

void InputManager::UpdateInput(SDL_Event& e)
{
    // If we don't check that it's a valid key we can accidentally bind it 
    if (!(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP || ValidKey(e.key.keysym.sym)))
        return;

    boundKeys[e.key.keysym.sym].state = e.type == SDL_KEYDOWN ? 1 : 0;
}

void InputManager::UpdatePrevInput()
{
    std::unordered_map<SDL_Keycode, Key>::iterator it = boundKeys.begin();
    while (it != boundKeys.end())
    {
        it->second.prevState = it->second.state;
        it++;
    }
}