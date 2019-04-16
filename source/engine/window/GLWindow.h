#pragma once

#include  <SDL.h>
#include "common/Config.h"
#include "math/Vector4.h"

namespace NTGS {
    class GLWindow {
    public:
        static std::shared_ptr<GLWindow> Create(const std::string& name, const Vec4i& geom);

        GLWindow();
        virtual ~GLWindow();

        void MainLoop();
        void Destroy();

    private:
        bool mStop = false;
        SDL_Window* mpWindow = nullptr;
        SDL_GLContext mpGLContext = nullptr;
    };
}