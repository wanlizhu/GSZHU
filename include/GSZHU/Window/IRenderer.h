#pragma once

#include <GSZHU/Config.h>
#include <GSZHU/IOEvents.h>

namespace GSZHU {
    // Note: double inheritance of enable_shared_from_this<> is dangerous 
    class IRenderer {
    public:
        virtual void OnInitialize() = 0;
        virtual void OnUpdate(std::shared_ptr<SWindowUpdateEvent> updateEvent) = 0;
        virtual void OnRender(std::shared_ptr<SWindowRenderEvent> renderEvent) = 0;
        virtual void OnResize(std::shared_ptr<SWindowResizeEvent> resizeEvent) = 0;
        virtual void OnDestroy() = 0;
    };
}