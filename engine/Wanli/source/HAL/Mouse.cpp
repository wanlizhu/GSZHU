#include "HAL/Mouse.h"
#include "Core/Engine.h"
#include "Utilities/Bitmap.h"
#include "glfw/glfw3.h"

namespace Wanli
{
    class CallbacksGLFW
    {
    public:
        static void OnMouseButton_GLFW(GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
        {
            Engine::Get()->GetModule<Mouse>()->OnButton(
                static_cast<EMouseButton>(button),
                static_cast<EInputAction>(action),
                static_cast<EInputMods>(mods)
            );
        }

        static void OnCursorPos_GLFW(GLFWwindow* window, double xpos, double ypos)
        {
            glm::dvec2 pos = { xpos, ypos };
            Engine::Get()->GetModule<Mouse>()->mPos = pos;
            Engine::Get()->GetModule<Mouse>()->OnPosition(pos);
        }

        static void OnCursorEnter_GLFW(GLFWwindow* window, int32_t entered)
        {
            Engine::Get()->GetModule<Mouse>()->mWindowSelected = entered == GLFW_TRUE;
            Engine::Get()->GetModule<Mouse>()->OnEnter(entered == GLFW_TRUE);
        }

        static void OnScroll_GLFW(GLFWwindow* window, double xoffset, double yoffset)
        {
            glm::dvec2 scroll = { xoffset, yoffset };
            Engine::Get()->GetModule<Mouse>()->mScroll = scroll;
            Engine::Get()->GetModule<Mouse>()->OnScroll(scroll);
        }

        static void OnDrop(GLFWwindow* window, int32_t count, const char** paths)
        {
            StringArray strArray(count);
            for (int i = 0; i < count; i++)
            {
                strArray.push_back(paths[i]);
            }
            Engine::Get()->GetModule<Mouse>()->OnDroppedFiles(strArray);
        }
    };

    Mouse::Mouse(GLFWwindow* window)
        : IModule(EModuleStage::PreUpdate, "Mouse")
        , mWindowGLFW(window)
    {
        glfwSetMouseButtonCallback(mWindowGLFW, CallbacksGLFW::OnMouseButton_GLFW);
        glfwSetCursorPosCallback(mWindowGLFW, CallbacksGLFW::OnCursorPos_GLFW);
        glfwSetCursorEnterCallback(mWindowGLFW, CallbacksGLFW::OnCursorEnter_GLFW);
        glfwSetScrollCallback(mWindowGLFW, CallbacksGLFW::OnScroll_GLFW);
        glfwSetDropCallback(mWindowGLFW, CallbacksGLFW::OnDrop);
    }

    Mouse::~Mouse()
    {
        if (mCursorGLFW)
        {
            glfwDestroyCursor(mCursorGLFW);
            mCursorGLFW = nullptr;
        }
    }

    void Mouse::Update()
    {
        double delta = Engine::Get()->GetEngineTimer().deltaUpdate.Get<Second>();

        // Updates the position delta
        mPosDelta = delta * (mLastPos - mPos);
        mLastPos = mPos;

        // Updates the scroll delta
        mScrollDelta = delta * (mLastScroll - mScroll);
        mLastScroll = mScroll;
    }

    EInputAction Mouse::GetButton(EMouseButton button) const
    {
        auto state = glfwGetMouseButton(mWindowGLFW, static_cast<int>(button));
        return static_cast<EInputAction>(state);
    }

    const glm::dvec2& Mouse::GetPos() const
    {
        return mPos;
    }

    const glm::dvec2& Mouse::GetPosDelta() const
    {
        return mPosDelta;
    }

    const glm::dvec2& Mouse::GetScroll() const
    {
        return mScroll;
    }

    const glm::dvec2& Mouse::GetScrollDelta() const
    {
        return mScrollDelta;
    }

    String Mouse::GetClipboard() const
    {
        return glfwGetClipboardString(mWindowGLFW);
    }

    void Mouse::SetCursor(const Path& path, ECursorHotSpot hotspot)
    {
        if (mCustomCursor && 
            mCustomCursor->first == path &&
            mCustomCursor->second == hotspot)
        {
            return;
        }

        Bitmap bitmap(path.string());
        if (!bitmap)
        {
            return;
        }

        GLFWimage image[1];
        image[0].width = bitmap.GetSize().x;
        image[0].height = bitmap.GetSize().y;
        image[0].pixels = bitmap.GetData().get();

        glfwDestroyCursor(mCursorGLFW);

        switch (hotspot)
        {
            case ECursorHotSpot::UpperLeft:
                mCursorGLFW = glfwCreateCursor(image, 0, 0);
                break;
            case ECursorHotSpot::UpperRight:
                mCursorGLFW = glfwCreateCursor(image, image->width - 1, 0);
                break; 
            case ECursorHotSpot::BottomLeft:
                mCursorGLFW = glfwCreateCursor(image, 0, image->height - 1);
                break;
            case ECursorHotSpot::BottomRight:
                mCursorGLFW = glfwCreateCursor(image, image->width - 1, image->height - 1);
                break;
            case ECursorHotSpot::Centered:
                mCursorGLFW = glfwCreateCursor(image, image->width /2, image->height /2);
                break;
            default:
                assert(false);
        }

        glfwSetCursor(mWindowGLFW, mCursorGLFW);
        mCustomCursor = { path, hotspot };
        mStandardCursor = std::nullopt;
    }

    void Mouse::SetCursor(EStandardCursor cursor)
    {
        if (mStandardCursor == cursor)
        {
            return;
        }

        glfwDestroyCursor(mCursorGLFW);

        mCursorGLFW = glfwCreateStandardCursor(static_cast<int>(cursor));

        glfwSetCursor(mWindowGLFW, mCursorGLFW);
        mCustomCursor = std::nullopt;
        mStandardCursor = cursor;
    }

    void Mouse::SetPos(const glm::dvec2& pos)
    {
        mLastPos = pos;
        mPos = pos;
        glfwSetCursorPos(mWindowGLFW, mPos.x, mPos.y);
    }

    void Mouse::SetScroll(const glm::dvec2& scroll)
    {
        mLastScroll = scroll;
        mScroll = scroll;
    }

    void Mouse::SetClipboard(const String& str) const
    {
        glfwSetClipboardString(mWindowGLFW, str.c_str());
    }

    bool Mouse::IsWindowSelected() const
    {
        return mWindowSelected;
    }

    bool Mouse::IsCursorVisible() const
    {
        return mCursorVisible;
    }

    void Mouse::SetCursorVisible(bool visible)
    {
        if (mCursorVisible != visible)
        {
            glfwSetInputMode(mWindowGLFW, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

            if (visible && !mCursorVisible)
            {
                SetPos(mPos);
            }
        }
        mCursorVisible = visible;
    }
}