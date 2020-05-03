#include "sample_application.h"

using namespace djv;

SampleApplication::SampleApplication(const djv::ApplicationConfig& config)
    : mArgList(config.argc, config.argv)
{
    mWindow = std::make_shared<Window>(
        config.windowDesc.title,
        config.windowDesc.size[0],
        config.windowDesc.size[1],
        config.windowDesc.callbacks,
        config.windowDesc.flags
    );
    mWindow->setWindowPos(config.windowDesc.pos[0], config.windowDesc.pos[1]);
    mWindow->setTickTimer(std::chrono::milliseconds(1000 / 60));

    mScene = createScene(std::filesystem::path(""));
}
    
SampleApplication::~SampleApplication()
{}

const djv::ArgList& SampleApplication::getArgList() const
{
    return mArgList;
}

djv::Window* SampleApplication::getWindow() const
{
    return mWindow.get();
}

djv::SceneInterface* SampleApplication::getScene() const
{
    return mScene.get();
}

const djv::Framerate& SampleApplication::getFramerate() const
{
    return mFramerate;
}

void SampleApplication::run()
{}

void SampleApplication::shutdown()
{}

void SampleApplication::resizeSwapchain(int width, int height)
{}

void SampleApplication::toggleUI(bool showUI)
{}

bool SampleApplication::isKeyPressed(int key) const
{
    return false;
}

void SampleApplication::captureScreen(const std::wstring& path)
{}

void SampleApplication::setScene(std::shared_ptr<djv::SceneInterface> scene)
{}

void SampleApplication::windowSizeChanged(int w, int h) 
{}

void SampleApplication::windowRedraw()
{}

void SampleApplication::windowClose()
{}

void SampleApplication::keyboardEvent(const djv::KeyboardEvent& keyboardEvent)
{}

void SampleApplication::mouseEvent(const djv::MouseEvent& mouseEvent)
{}
