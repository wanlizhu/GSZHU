#pragma once

#include "GE2/Utilities.h"
#include "WindowEvent.h"
#include "../SignalSlot.h"

struct GLFWwindow;

namespace GE2::RHI
{
    enum class EWindowFlags : int
    {
        None = 0,
        Fullscreen,
        Resizable,
        AcceptDropFiles,
    };
    DEFINE_ENUM_OPS(EWindowFlags)

    class GE2_IMPEXP Window
    {
    public:
        using EventHandler = std::function<void(uint32_t, void*)>;

        Window();
        virtual ~Window();

        bool Create(const char* title, int w, int h, EWindowFlags flags = EWindowFlags::None);
        void Close() const;

        void SetVisible(bool value=true) const;
        void SetSize(int w, int h) const;
        void SetPos(int x, int y) const;
        void SetTitle(const char* title) const;
        void SetUserPointer(void* pointer) const;

        INT2  GetSize() const;
        INT2  GetPos() const; 
        void* GetHandle() const;
        void* GetUserPointer() const;

        void  SendEvent(uint32_t event, void* data);
        void  InstallEventFilter(uint32_t event, const EventHandler& handler);

    public:
        Signal<void(double)>      OnUpdate;
        Signal<void()>            OnDraw;
        Signal<void(int, int)>    OnResize;
        Signal<void(KeyEvent)>    OnKeyEvent;
        Signal<void(MouseEvent)>  OnMouseEvent;
        Signal<void(STRLIST)>     OnDropFile;

    private:
        GLFWwindow* mpWindow = nullptr;  
        std::unordered_map<uint32_t, EventHandler> mEventFilters;
    };
}