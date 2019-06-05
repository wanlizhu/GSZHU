#pragma once

#include <GSZHU/IEvent.h>
#include <GSZHU/EKeyCode.h>
#include <stdint.h>

namespace GSZHU {
    struct SModifierSet {
        bool control = false;
        bool shift = false;
        bool alt = false;
    };

    struct SMouseButtonSet {
        bool left = false;
        bool right = false;
        bool middle = false;
    };

    enum class EMouseButton : int {
        None,
        Left,
        Right,
        Middle
    };

    enum class EKeyState : int {
        Up,
        Down,
        DoubleClick,
    };

    struct SKeyEvent : public IEvent {
        EKeyCode key = EKeyCode::None;
        uint32_t unicode = 0;
        EKeyState state = EKeyState::Up;
        SModifierSet mods;

        virtual EType GetType() const override { return EType::Key; }

    };

    struct SMouseMotionEvent : public IEvent {
        int x = 0;
        int y = 0;
        int xOffset = 0; // How far the mouse moved since the last event.
        int yOffset = 0;
        SMouseButtonSet buttons;
        SModifierSet mods;

        virtual EType GetType() const override { return EType::MouseMotion; }
    };

    struct SMouseButtonEvent : public IEvent {
        int x = 0;
        int y = 0;
        EMouseButton button;
        EKeyState state;
        SModifierSet mods;
        
        virtual EType GetType() const override { return EType::MouseButton; }
    };

    struct SMouseWheelEvent : public IEvent {
        int x = 0; // The cursor relative to the upper-left corner of the client area
        int y = 0;
        float delta = 0.f;
        SMouseButtonSet buttons;
        SModifierSet mods;
        
        virtual EType GetType() const override { return EType::MouseWheel; }
    };

    struct SWindowResizeEvent : public IEvent {
        int width = 0;
        int height = 0;

        virtual EType GetType() const override { return EType::WindowResize; }
    };

    struct SWindowUpdateEvent : public IEvent {
        double deltaTime = 0.0;
        double totalTime = 0.0;
        uint64_t frameNumber = 0;

        virtual EType GetType() const override { return EType::WindowUpdate; }
    };

    struct SWindowRenderEvent : public IEvent {
        double deltaTime = 0.0;
        double totalTime = 0.0;
        uint64_t frameNumber = 0;

        virtual EType GetType() const override { return EType::WindowRender; }
    };

    struct SWindowDestroyEvent : public IEvent {
        virtual EType GetType() const override { return EType::WindowDestroy; }
    };
}