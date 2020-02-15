#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "HAL/UserInputFlags.h"
#include "Utilities/Delegate.h"

struct GLFWwindow;
struct GLFWcursor;

namespace Wanli
{
    class DLLDECL Mouse : public IModule
    {
        friend class CallbacksGLFW;
    public:
        Mouse(GLFWwindow* window);
        virtual ~Mouse();

        virtual void Update() override;
        EInputAction GetButton(EMouseButton button) const;
        const glm::dvec2& GetPos() const;
        const glm::dvec2& GetPosDelta() const;
        const glm::dvec2& GetScroll() const;
        const glm::dvec2& GetScrollDelta() const;
        String GetClipboard() const;

        void SetCursor(const Path& path, ECursorHotSpot hotspot = ECursorHotSpot::Centered);
        void SetCursor(EStandardCursor cursor);
        void SetPos(const glm::dvec2& pos);
        void SetScroll(const glm::dvec2& scroll);
        void SetClipboard(const String& str) const;

        bool IsWindowSelected() const;
        bool IsCursorVisible() const;
        void SetCursorVisible(bool visible);

    public:
        Delegate<void(EMouseButton, EInputAction, EInputMods)> OnButton;
        Delegate<void(glm::vec2)> OnPosition;
        Delegate<void(glm::vec2)> OnScroll;
        Delegate<void(bool)> OnEnter;
        Delegate<void(StringArray)> OnDroppedFiles;

    private:
        GLFWwindow* mWindowGLFW = nullptr;
        GLFWcursor* mCursorGLFW = nullptr;
        Optional<std::pair<Path, ECursorHotSpot>> mCustomCursor;
        Optional<EStandardCursor> mStandardCursor;
        bool mWindowSelected = false;
        bool mCursorVisible = true;

        glm::dvec2 mLastPos;
        glm::dvec2 mPos;
        glm::dvec2 mPosDelta;

        glm::dvec2 mLastScroll;
        glm::dvec2 mScroll;
        glm::dvec2 mScrollDelta;
    };
}