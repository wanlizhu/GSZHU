#pragma once

#include  <SDL.h>
#include "window/IWindow.h"
#include "common/Config.h"
#include "math/Vector4.h"
#include "math/Vector2.h"

namespace NTGS {
    class GLWindow : public IWindow {
    public:
        static std::shared_ptr<GLWindow> Create(const std::string& name, const Vec4i& geom);

        GLWindow();
        virtual ~GLWindow();

        void MainLoop();
        void Destroy();
        Vec2i GetPosition() const;
        Vec2i GetSize() const;
        void SetPosition(const Vec2i& pos);
        void SetSize(const Vec2i& size);

    private:
        bool mStop = false;
        SDL_Window* mpWindow = nullptr;
        SDL_GLContext mpGLContext = nullptr;
    };
}