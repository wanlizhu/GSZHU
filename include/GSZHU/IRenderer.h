#pragma once

#include <GSZHU/Config.h>
#include <GSZHU/IOEvents.h>

namespace GSZHU {
    // Note: double inheritance of enable_shared_from_this<> is dangerous 
    class IRenderer {
    public:
        virtual void OnInitialize() = 0;
        virtual void OnUpdate(SWindowUpdateEvent updateEvent) = 0;
        virtual void OnRender(SWindowRenderEvent renderEvent) = 0;
        virtual void OnResize(SWindowResizeEvent resizeEvent) = 0;
        virtual void OnDestroy() = 0;
    };
}