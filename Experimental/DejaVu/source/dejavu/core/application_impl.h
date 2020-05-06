#pragma once

#include "core/application_interface.h"
#include "utils/inherit_shared_from_this.h"

namespace djv
{
    class ApplicationImpl 
        : public ApplicationInterface
        , public WindowCallbacks
        , public inherit_shared_from_this<ApplicationInterface, ApplicationImpl>
    {
    public:
        using inherit_shared_from_this<ApplicationInterface, ApplicationImpl>::shared_from_this;

        ApplicationImpl(const ApplicationConfig& config);
        ~ApplicationImpl();

        virtual const ArgList& getArgList() const override final;
        virtual Window* getWindow() const override final;
        virtual const Framerate& getFramerate() const override final; 

        virtual void run() override final;
        virtual void shutdown() override final;
        virtual void resizeSwapchain(int width, int height) override final;
        virtual void toggleUI(bool showUI) override final;
        virtual bool isKeyPressed(int key) const override final;
        virtual void captureScreen(const std::wstring& path) override final;
        virtual void setScene(std::shared_ptr<SceneInterface> scene) override final;
        virtual void setRenderer(std::shared_ptr<RendererInterface> renderer) override final;

    private:
        virtual void windowSizeChanged(int w, int h) override final;
        virtual void windowRedraw() override final;
        virtual void windowClose() override final;
        virtual void keyboardEvent(const KeyboardEvent& keyboardEvent) override final;
        virtual void mouseEvent(const MouseEvent& mouseEvent) override final;

        void runInternal();

    private:
        ArgList mArgList;
        Framerate mFramerate;
        std::shared_ptr<DeviceInterface>   mpDevice;
        std::shared_ptr<RendererInterface> mpRenderer;
        std::shared_ptr<SceneInterface>    mpScene;
        std::shared_ptr<Window>            mpWindow;
    };
}