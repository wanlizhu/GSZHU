#pragma once

#include "BasicTypes.h"
#include "IModule.h"
#include "Utilities/CommandLineArgs.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/ElapsedTime.h"
#include "Utilities/ChangePerSecond.h"
#include "Utilities/Future.h"
#include "VulkanRHI/VulkanRHI.h"
#include "Messages/MessageQueue.h"
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
        bool Initialize(CommandLineArgs commandLineArgs,
                        WindowHandle window,
                        EDeviceType deviceType,
                        EFeatureFlags enabledFeatures,
                        Color backgroundColor = Color_AutoCAD,
                        const Array<Path>& assertPaths = {},
                        ErrorCallback errorCallback = nullptr);
        int Run();
        Future<int> RunAsync();

        inline void Shutdown() { mRunning = false; }
        inline void SetFpsLimit(float limit) { mFpsLimit = limit; }

        inline VulkanRHI* GetVulkanRHI() const { return mVulkanRHI.get(); }
        inline Uint GetFps() const { return mFpsRecord.GetValue(); }
        inline Uint GetUps() const { return mUpsRecord.GetValue(); }
        inline Optional<float> GetFpsLimit() const { return mFpsLimit; }
        inline bool IsRunning() const { return mRunning; }
        inline const Version& GetVersion() const { return mVersion; }
        inline const String& GetAppName() const { return mAppName; }
        inline const CommandLineArgs& GetCommandLineArgs() const { return mCommandLineArgs; }
        inline WindowHandle GetWindowHandle() const { return mWindowHandle; }

    protected:
        Engine();
        void Destroy();
        void LoadConfig();
        void LoadPlugins();
        void UpdateStage(EModuleStage stage);

    private:
        bool mRunning = false;
        Version mVersion;
        String mAppName;
        CommandLineArgs mCommandLineArgs;
        UniquePtr<VulkanRHI> mVulkanRHI;
        WindowHandle mWindowHandle = nullptr;

        ElapsedTime mElapsedUpdate;
        ElapsedTime mElapsedRender;

        Optional<float> mFpsLimit;
        ChangePerSecond mFpsRecord;
        ChangePerSecond mUpsRecord;
    };
}