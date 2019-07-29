#pragma once

#include "Engine/Common/Config.h"
#include "Engine/Common/NonCopyable.h"
#include "Engine/GUI/WindowBase.h"
#include <any>
#include <memory>
#include <string>
#include <list>
#include <unordered_map>
#include <functional>

struct GLFWwindow;

namespace ZHU
{
    class ZHU_API Window : public WindowBase, public NonCopyable
    {
    public:
        Window();
        Window(const CreationInfo& info);
        virtual ~Window();

        static INT2 DefaultPos();
        static INT2 DefaultSize();

        virtual void SetTitle(PCCH title) override;
        virtual void SetFullScreen(bool enabled = true) override;
        virtual void SetSize(int width, int height) override;
        virtual void MoveTo(int x, int y) override;
        virtual void MainLoop() override;
        virtual void Hide() override;
        virtual void Show() override;
        virtual void Destroy() override;

        virtual PCCH GetTitle() const override;
        virtual INT2 GetPosition() const override;
        virtual INT2 GetSize() const override;
        
    protected:
        static void Initialize();

    private:
        GLFWwindow* mpWindow = nullptr;
    };
      
}