#pragma once

#include "core/application_interface.h"

class SampleApplication 
    : public djv::ApplicationInterface
    , public djv::WindowCallbacks
{
public:
    SampleApplication(const djv::ApplicationConfig& config);
    ~SampleApplication();
    
    virtual const djv::ArgList& getArgList() const override final;
    virtual djv::Window* getWindow() const override final;
    virtual djv::SceneInterface* getScene() const override final;
    virtual const djv::Framerate& getFramerate() const override final; 
    
    virtual void run() override final;
    virtual void shutdown() override final;
    virtual void resizeSwapchain(int width, int height) override final;
    virtual void toggleUI(bool showUI) override final;
    virtual bool isKeyPressed(int key) const override final;
    virtual void captureScreen(const std::wstring& path) override final;
    virtual void setScene(std::shared_ptr<djv::SceneInterface> scene) override final;

private:
    virtual void windowSizeChanged(int w, int h) override final;
    virtual void windowRedraw() override final;
    virtual void windowClose() override final;
    virtual void keyboardEvent(const djv::KeyboardEvent& keyboardEvent) override final;
    virtual void mouseEvent(const djv::MouseEvent& mouseEvent) override final;

private:
    djv::ArgList mArgList;
    djv::Framerate mFramerate;
    std::shared_ptr<djv::Window> mWindow;
    std::shared_ptr<djv::SceneInterface> mScene;
};