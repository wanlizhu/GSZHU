#pragma once

#include "common/Signal.h"

namespace NTGS {
    class IWindow {
    public:
        enum Action {
            PRESSED = 1,
            RELEASED = 2,
            REPEATED = 4,
        };

        enum MouseButton {
            MOUSE_LEFT = 8,
            MOUSE_MIDDLE = 16,
            MOUSE_RIGHT = 32,
        };

        enum ModKey {
            MOD_SHIFT = 1,
            MOD_CTRL = 2,
            MOD_ALT = 4,
        };

        struct MouseButtonEvent {
            MouseButton button = MOUSE_LEFT;
            Action action = Action(0);
            int clicks = 1; // 1 for single-click, 2 for double-click
            int x = 0, y = 0; // relative to window
        };

        struct MouseMotionEvent {
            int x = 0;
            int y = 0;
            int xdiff = 0;
            int ydiff = 0;
        };

        struct MouseWheelEvent {
            int x = 0;
            int y = 0;
        };
             
        struct KeyEvent {
            int key = 0;
            Action action = Action(0);
            ModKey mods = ModKey(0);
        };

    public:
        Signal<void()> SIGQuit;
        Signal<void(int, int)> SIGResize;
        Signal<void(MouseButtonEvent)> SIGMouseButton;
        Signal<void(MouseMotionEvent)> SIGMouseMotion;
        Signal<void(MouseWheelEvent)> SIGMouseWheel;
        Signal<void(KeyEvent)> SIGKey;
    };
}