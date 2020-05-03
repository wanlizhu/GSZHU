#pragma once

#include "core/window.h"
#include "scene/scene_interface.h"
#include "device/device_interface.h"
#include "renderer/renderer_interface.h"
#include "utils/preprocess.h"
#include "utils/clock.h"
#include "utils/framerate.h"
#include "utils/arg_list.h"

#ifdef _DEBUG
#define DEFAULT_ENABLE_DEBUG_LAYER true
#else
#define DEFAULT_ENABLE_DEBUG_LAYER false
#endif

namespace djv
{
    struct ApplicationConfig
    {
        struct {
            const wchar_t* title = L"DejaVu";
            std::array<int, 2> size;
            std::array<int, 2> pos;
            std::weak_ptr<WindowCallbacks> callbacks;
            WindowFlag flags = WindowFlag::None;
        } windowDesc;

        int    argc = 0;
        char** argv = nullptr;
        bool enableAlertDialog = true;
        bool enableUI = true;

        std::filesystem::path scenePath;
    };

    class DJV_API ApplicationInterface : public std::enable_shared_from_this<ApplicationInterface>
    {
    public:
        virtual const ArgList& getArgList() const = 0;
        virtual Window* getWindow() const = 0;
        virtual const Framerate& getFramerate() const = 0; 
        
        virtual void run() = 0;
        virtual void shutdown() = 0;
        virtual void resizeSwapchain(int width, int height) = 0;
        virtual void toggleUI(bool showUI) = 0;
        virtual bool isKeyPressed(int key) const = 0;
        virtual void captureScreen(const std::wstring& path) = 0;
        virtual void setScene(std::shared_ptr<SceneInterface> scene) = 0;
        virtual void setRenderer(std::shared_ptr<RendererInterface> renderer) = 0;
    };

    DJV_API extern std::weak_ptr<ApplicationInterface> gpApp;
}