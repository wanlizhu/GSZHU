#pragma once

#include "BasicTypes.h"
#include "IModule.h"
#include "Plugin.h"
#include "Utilities/CommandLineArgs.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/ElapsedTime.h"
#include "Utilities/ChangePerSecond.h"
#include "Utilities/Future.h"
#include "VulkanRHI/VulkanRHI.h"
#include "HAL/NativeHandles.h"

namespace Wanli
{
    enum class EDeviceType
    {
        Default = 0,
        D3D12,
        Vulkan,
        Metal,
        OpenGL,
    };

    enum EFeatureFlags
    {
        FF_All = 0,
        FF_LineAA = 1,
        FF_PBR = (1 << 1),
        FF_CPURayTracing = (1 << 2),
        FF_GPURayTracing = (1 << 3),
        FF_MeshShader = (1 << 4),
    };

    class DLLDECL Engine : public NonCopyable
    {
    public:
        using ErrorCallback = void(__stdcall*)(Uint, const char*);

        static Engine* Get();
        
        ~Engine();
        bool Initialize(const CommandLineArgs& commandLineArgs);
        int Run();
        Future<int> RunAsync();

        inline const Version& GetVersion() const { return mVersion; }
        inline Window* GetWindow() const { return mWindow.get(); }
        inline IApplication* GetApp() const { return mApp.get(); }
        inline VulkanRHI* GetVulkanRHI() const { return mVulkanRHI.get(); }

    private:
        Engine();
        void Destroy();
        void LoadConfig();
        void LoadPlugins();
        void UpdateStage(EModuleStage stage);

    private:
        bool mRunning = false;
        Version mVersion;
        UniquePtr<IApplication> mApp;
        UniquePtr<Window> mWindow;
        UniquePtr<VulkanRHI> mVulkanRHI;
        Array<UniquePtr<Plugin>> mPlugins;

        ElapsedTime mElapsedUpdate;
        ElapsedTime mElapsedRender;
        Optional<float> mFpsLimit;
        ChangePerSecond mFpsRecord;
        ChangePerSecond mUpsRecord;
    };
}