#include "HAL/InputManager.h"

namespace Wanli
{
    InputManager* InputManager::Get()
    {
        InputManager instance;
        return &instance;
    }

    InputManager::InputManager()
    {}

    InputManager::~InputManager()
    {}
}