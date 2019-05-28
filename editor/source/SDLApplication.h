#pragma once

#include "SDLError.h"
#include "InputEvent.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <map>
#include <cassert>
#include <SDL.h>

class SDLApplication {
public:
    using OnWindowEventFunc = std::function<void(const SDL_WindowEvent&)>;
    using OnRedrawEventFunc = std::function<void(void)>;
    
    using OnMouseMoveEventFunc = std::function<void(int, int, int)>;
    using OnMouseDownEventFunc = std::function<void(MouseEvent::MouseButton, int, int)>;
    using OnMouseUpEventFunc = std::function<void(MouseEvent::MouseButton, int, int)>;
    using OnKeyDownEventFunc = std::function<void(const SDL_Keycode&)>;
    using OnKeyUpEventFunc = std::function<void(const SDL_Keycode&)>;

    SDLApplication() = default;
    virtual ~SDLApplication();

    virtual bool Initialize(uint32_t subsys);
    virtual void Run();
    virtual void Destroy();

    virtual void RegisterWindowEvent(const SDL_Window* win, const WindowEvent* cb);
    virtual void RegisterMouseEvent(const SDL_Window* win, const MouseEvent* cb);
    virtual void RegisterKeyboardEvent(const SDL_Window* win, const KeyboardEvent* cb);

private:
    std::unordered_map<const SDL_Window*, OnWindowEventFunc> mOnWindowCallbacks;
    std::unordered_map<const SDL_Window*, OnRedrawEventFunc> mOnRedrawCallbacks;
    std::unordered_map<const SDL_Window*, OnMouseMoveEventFunc> mOnMouseMoveCallbacks;
    std::unordered_map<const SDL_Window*, OnMouseDownEventFunc> mOnMouseDownCallbacks;
    std::unordered_map<const SDL_Window*, OnMouseUpEventFunc> mOnMouseUpCallbacks;
    std::unordered_map<const SDL_Window*, OnKeyDownEventFunc> mOnKeyDownCallbacks;
    std::unordered_map<const SDL_Window*, OnKeyUpEventFunc> mOnKeyUpCallbacks;
};
