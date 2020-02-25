#include "Inputs/InputManager.h"
#ifdef PLATFORM_WINDOWS
#include "Windows/WindowsInputManager.h"
#endif

namespace Wanli
{
    InputManager::InputManager()
#ifdef PLATFORM_WINDOWS
        : mImpl(new WindowsInputManager())
#elif defined(__ANDROID__)
        : mImpl(new AndroidInputManager())
#endif
    {}

    InputManager::~InputManager()
    {
        Destroy();
    }

    void InputManager::Initialize()
    {
        mImpl->Initialize();
    }

    void InputManager::Update()
    {
        mImpl->Update();
    }

    void InputManager::Destroy()
    {
        mImpl->Destroy();
    }
}