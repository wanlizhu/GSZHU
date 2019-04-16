#include "window/GLWindow.h"
#include "glad/glad.h"

namespace NTGS {
    bool Init_SDL_Subsystems() { 
        uint32_t subsys = SDL_WasInit(SDL_INIT_EVERYTHING);

        if (!(subsys & SDL_INIT_VIDEO)) {
            SDL_Init(SDL_INIT_VIDEO);
        }
        if (!(subsys & SDL_INIT_TIMER)) {
            SDL_Init(SDL_INIT_TIMER);
        }
        if (!(subsys & SDL_INIT_EVENTS)) {
            SDL_Init(SDL_INIT_EVENTS);
        }

        return true;
    }

    std::shared_ptr<GLWindow> GLWindow::Create(const std::string& name, const Vec4i& geom) {
        Init_SDL_Subsystems();
        std::shared_ptr<GLWindow> win = std::make_shared<GLWindow>();

        SDL_GL_LoadLibrary(NULL);
        SDL_GL_SetSwapInterval(1); // enable v-sync
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        win->mpWindow = SDL_CreateWindow(name.c_str(), 
            geom.x == -1 ? SDL_WINDOWPOS_CENTERED : geom.x,
            geom.y == -1 ? SDL_WINDOWPOS_CENTERED : geom.y, geom.z, geom.w, SDL_WINDOW_OPENGL);
        if (!win->mpWindow) {
            printf("error: %s\n", SDL_GetError());
            return win;
        }

        win->mpGLContext = SDL_GL_CreateContext(win->mpWindow);

        gladLoadGLLoader(SDL_GL_GetProcAddress);
        printf("Vendor:   %s\n", glGetString(GL_VENDOR));
        printf("Renderer: %s\n", glGetString(GL_RENDERER));
        printf("Version:  %s\n", glGetString(GL_VERSION));

        return win;
    }

    GLWindow::GLWindow() {

    }

    GLWindow::~GLWindow() {
        Destroy();
    }

    void GLWindow::Destroy() {
        if (mpGLContext) {
            SDL_GL_DeleteContext(mpGLContext);
            mpGLContext = nullptr;
        }

        if (mpWindow) {
            SDL_DestroyWindow(mpWindow);
            mpWindow = nullptr;
        }

        SDL_Quit();
    }

    void GLWindow::MainLoop() {
        while (!mStop) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    mStop = true;
                    return;
                }

                if (event.type == SDL_KEYDOWN) {

                }
            }

            SDL_GL_SwapWindow(mpWindow);
        }
    }
}