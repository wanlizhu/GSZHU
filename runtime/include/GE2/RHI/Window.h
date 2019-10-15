#pragma once

#include "BasicTypes.h"
#include "WindowEvent.h"
#include "../SignalSlot.h"

struct GLFWwindow;

namespace RHI
{
    class RHI_API Window
    {
    public:
        using EventHandler = std::function<void(uint32_t, void*)>;

        Window();
        virtual ~Window();

        bool Create(const char* title, int w, int h, uint32_t flags = 0);
        bool IsActive() const;

        void SetVisible(bool value=true) const;
        void SetSize(int w, int h) const;
        void SetPos(int x, int y) const;
        void SetTitle(const char* title) const;

        std::string GetTitle() const;
        INT2  GetSize() const;
        INT2  GetPos() const; 
        void* GetHandle() const;
        void* GetHDC() const;

        void  SendEvent(uint32_t event, void* data);
        void  InstallEventFilter(uint32_t event, const EventHandler& handler);

    public:
        Signal<void(double)>      OnUpdate;
        Signal<void()>            OnDraw;
        Signal<void(int, int)>    OnResize;
        Signal<void(KeyEvent)>    OnKeyEvent;
        Signal<void(MouseEvent)>  OnMouseEvent;
        Signal<void(std::string)> OnDropFile;

    private:
        GLFWwindow* mWindow = nullptr;  
        std::unordered_map<uint32_t, EventHandler> mEventFilters;
    };
}