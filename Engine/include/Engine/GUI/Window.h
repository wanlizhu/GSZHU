#pragma once

#include "Engine/Common/Config.h"
#include "Engine/Common/NonCopyable.h"
#include "Engine/Math/Math.h"
#include <any>
#include <memory>
#include <string>
#include <list>
#include <unordered_map>
#include <functional>

struct GLFWwindow;

namespace ZHU
{
#define SCREEN_CENTER Vec2i(-1, -1)
#define SCREEN_HALF   Vec2i(-2, -2)

    class ZHU_API Window : public NonCopyable
    {
    public:
        Window();
        Window(const char* title, const Vec2i& size, const Vec2i& pos, bool fullscreen = false);
        virtual ~Window();

        static void Initialize();
        static void SetCreationHint(int hint, int value);
        
        void SetFullScreen(bool enabled);
        void SetSize(int width, int height);
        void MoveTo(int x, int y);
        void SetAttrib(int attrib, int value);
        void MainLoop();
        void Destroy();

        GLFWwindow* GetID() const;
        Vec2i GetPosition() const;
        Vec2i GetSize() const;
        
    private:
        GLFWwindow* mpWindow = nullptr;
    };

}