#pragma once

#include <GSZHU/CBuilderBase.h>
#include <GSZHU/Config.h>
#include <GSZHU/CManagedAPI.h>
#include <GSZHU/IRenderer.h>

namespace GSZHU {
    class IWindow : public CManagedAPI<IWindow> {
    public:
        static constexpr int NumBackBuffers = 3;

        IWindow(STRING const& Name, INT4 const& Geom, bool VSync)
            : mName(Name)
            , mGeometry(Geom)
            , mVSync(VSync)
        {}

        virtual void* GetNativeHandle() const = 0;
        virtual STRING const& GetName() const = 0;
        virtual void Initialize() = 0;
        virtual void Destroy() = 0;
        virtual INT2 GetSize() const = 0;
        virtual INT2 GetPosition() const = 0;

        virtual bool IsValid() const = 0;
        virtual bool IsVSync() const = 0;
        virtual void SetVSync(bool Enabled) = 0;
        virtual bool IsFullScreen() const = 0;
        virtual void SetFullScreen(bool Enabled) = 0;
        
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void SetUserInputHandler(std::weak_ptr<IUserInput> Handler) = 0;

    protected:
        STRING mName;
        INT4 mGeometry = { 0, 0, 0, 0 };
        bool mVSync = false;
        bool mFullScreen = false;
        int mBackBufferIndex = 0;
        uint64_t mFrameCounter = 0;

        std::weak_ptr<IUserInput> mUserInputHandler = nullptr;
    };
}