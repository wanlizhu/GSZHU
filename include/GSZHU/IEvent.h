#pragma once

#include <GSZHU/Config.h>

namespace GSZHU {
    class IEvent {
    public:
        enum class EType : int {
            Key,
            MouseMotion,
            MouseButton,
            MouseWheel,
            WindowResize,
            WindowUpdate,
            WindowRender,
            WindowDestroy,
        };

        virtual EType GetType() const = 0;
    };
}