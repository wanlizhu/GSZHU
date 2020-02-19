#include "Application/IApplication.h"
#include "Application/WindowsApplication.h"

namespace Wanli
{
#ifdef PLATFORM_WINDOWS
    UniquePtr<IApplication> MakeApplication(const WindowCreateInfo& info)
    {
        auto app = new WindowsApplication();
        app->Initialize(info);
        return std::unique_ptr<IApplication>(app);
    }
#else
#error "!!! Unsupported platform !!!"
#endif
}