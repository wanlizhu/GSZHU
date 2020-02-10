#pragma once

#include <glm/glm.h>
#include "HAL/EKeyType.h"
#include "HAL/EMouseType.h"

namespace Wanli
{
    class IEventListener
    {
    public:
        virtual bool OnKeyDown(EKeyType key) const { return false; };
        virtual bool OnKeyUp(EKeyType key) const { return false; }
        virtual bool OnMouseDown(EMouseType button, const glm::vec2& pos) const { return false; }
        virtual bool OnMouseUp(EMouseType button, const glm::vec2& pos) const { return false; }
        virtual bool OnMouseDoubleClick(EMouseType button, const glm::vec2& pos) const { return false; }
        virtual bool OnMouseWheel(float delta, const glm::vec2& pos) const { return false; }
        virtual bool OnMouseMove(const glm::vec2& pos) const { return false; }
        virtual bool OnTouchBegin(const std::vector<glm::vec2>& locations) const { return false; }
        virtual bool OnTouchMove(const std::vector<glm::vec2>& locations) const { return false; }
        virtual bool OnTouchEnd(const std::vector<glm::vec2>& locations) const { return false; }
        virtual bool OnTouchForceChanged(const std::vector<glm::vec2>& locations) const { return false; }
        virtual bool OnTouchFirstMove(const std::vector<glm::vec2>& locations) const { return false; }
        virtual void OnWindowSizeChanged(int width, int height) const {}
        virtual void OnWindowDraw() const {}
        virtual void OnWindowResizeBegin() const {}
        virtual void OnWindowResizeEnd() const {}
        virtual void OnWindowResizing() const {}
        virtual void OnWindowMove(int x, int y) const {}
        virtual void OnWindowClose() const {}
        virtual void OnDPIChanged() const {}
        virtual void OnApplicationQuit() const {}
    };
}