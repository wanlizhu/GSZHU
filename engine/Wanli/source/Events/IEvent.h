#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"
#include "Utilities/UniqueID.h"
#include "Core/Engine.h"

namespace Wanli
{
    enum EEventType
    {
        EVT_WindowClose,
        EVT_WindowRedraw,
        EVT_WindowSize,
        EVT_WindowPos,
        EVT_WindowFocus,
        EVT_WindowIconified,
        EVT_WindowMenu,
        EVT_FramebufferSize,
        EVT_Keyboard,
        EVT_KeyboardChar,
        EVT_MouseButton,
        EVT_MouseWheel,
        EVT_MouseMove,
        EVT_MouseEnter,
    };

    class DLLDECL IEvent : public NonCopyable
        , public std::enable_shared_from_this<IEvent>
    {
    public:
        virtual Uint GetID() const = 0;
        virtual EEventType GetEventType() const = 0;
        virtual const std::any& GetData() const = 0;
    }; 
}