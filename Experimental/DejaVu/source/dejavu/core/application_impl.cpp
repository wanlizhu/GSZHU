#include "core/application_impl.h"
#include "utils/logger.h"

namespace djv
{
    std::weak_ptr<ApplicationInterface> gpApp;

    ApplicationImpl::ApplicationImpl(const djv::ApplicationConfig& config)
        : mArgList(config.argc, config.argv)
    {
        google::InitGoogleLogging(config.argv[0]); 
        FLAGS_colorlogtostderr = true;
        FLAGS_logbufsecs = 0;
        FLAGS_max_log_size = 1024; 
        FLAGS_stop_logging_if_full_disk = true;

        mpWindow = std::make_shared<Window>(
            config.windowDesc.title,
            config.windowDesc.size[0],
            config.windowDesc.size[1],
            config.windowDesc.callbacks,
            config.windowDesc.flags
        );
        LOG_IF(ERROR, mpWindow == nullptr) << "Failed to create window";
        mpWindow->setWindowPos(config.windowDesc.pos[0], config.windowDesc.pos[1]);
        mpWindow->setTickTimer(std::chrono::milliseconds(1000 / 60));

        mpScene = createScene(config.scenePath);
        LOG_IF(ERROR, mpScene == nullptr) << "Failed to create scene";

        mpDevice = createDevice();
        LOG_IF(ERROR, mpDevice == nullptr) << "Failed to create device";

        mpRenderer = createRenderer();
        LOG_IF(ERROR, mpRenderer == nullptr) << "Failed to create renderer";
    }

    ApplicationImpl::~ApplicationImpl()
    {
        google::ShutdownGoogleLogging();
    }

    const djv::ArgList& ApplicationImpl::getArgList() const
    {
        return mArgList;
    }

    djv::Window* ApplicationImpl::getWindow() const
    {
        return mpWindow.get();
    }

    const djv::Framerate& ApplicationImpl::getFramerate() const
    {
        return mFramerate;
    }

    void ApplicationImpl::run()
    {
        try {
            runInternal();
        } catch (const std::exception& err) {
            LOG(ERROR) << err.what();
        }
    }

    void ApplicationImpl::runInternal()
    {
        gpApp = std::static_pointer_cast<ApplicationInterface>(shared_from_this());

    }

    void ApplicationImpl::shutdown()
    {}

    void ApplicationImpl::resizeSwapchain(int width, int height)
    {}

    void ApplicationImpl::toggleUI(bool showUI)
    {}

    bool ApplicationImpl::isKeyPressed(int key) const
    {
        return false;
    }

    void ApplicationImpl::captureScreen(const std::wstring& path)
    {}

    void ApplicationImpl::setScene(std::shared_ptr<djv::SceneInterface> scene)
    {}

    void ApplicationImpl::setRenderer(std::shared_ptr<RendererInterface> renderer)
    {}

    void ApplicationImpl::windowSizeChanged(int w, int h) 
    {}

    void ApplicationImpl::windowRedraw()
    {}

    void ApplicationImpl::windowClose()
    {}

    void ApplicationImpl::keyboardEvent(const djv::KeyboardEvent& keyboardEvent)
    {}

    void ApplicationImpl::mouseEvent(const djv::MouseEvent& mouseEvent)
    {}
}