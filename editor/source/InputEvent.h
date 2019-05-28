#pragma once

#include <SDL.h>

class WindowEvent {
public:
    enum UserEventType {
        USER_EVENT_REDRAW = SDL_USEREVENT + 1,
        USER_EVENT_COUNT
    };

    virtual ~WindowEvent() = default;

    virtual void OnWindowEvent(SDL_WindowEvent* event) = 0;
    virtual void OnRedraw() = 0;
};


class MouseEvent {
public:
    enum MouseButton {
        LEFT = 0,
        MIDDLE = 1,
        RIGHT = 2,
    };

    virtual ~MouseEvent() = default;

    virtual void OnMouseMove(int x, int y, int flags) = 0;
    virtual void OnMouseButtonDown(MouseButton btn, int x, int y) = 0;
    virtual void OnMouseButtonUp(MouseButton btn, int x, int y) = 0;
};


class KeyboardEvent {
public:
    virtual ~KeyboardEvent() = default;

    virtual void OnKeyDown(const SDL_Keycode& key) = 0;
    virtual void OnKeyUp(const SDL_Keycode& key) = 0;
};