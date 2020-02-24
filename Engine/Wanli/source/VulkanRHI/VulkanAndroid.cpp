#if defined(__ANDROID__)

#include "VulkanAndroid.h"
#include <dlfcn.h>

namespace Wanli
{
    android_app* gpAndroidApp = nullptr;
    int gnScreenDensity = 1;

    static void* spVulkanDL = nullptr;

    bool LoadVulkanLibrary()
    {

    }

    void LoadVulkanFunctions();
    void FreeVulkanLibrary();
    void ShowAlert(const char* msg);
}

#endif