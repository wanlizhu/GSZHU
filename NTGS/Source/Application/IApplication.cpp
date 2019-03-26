#include "Application/IApplication.h"

#ifdef _WIN32
#include "Application/Application_Win32.h"
#endif

namespace NTGS {
    IApplicationPtr IApplication::Create() {
#ifdef _WIN32
        std::shared_ptr<Application_Win32> app = Application_Win32::Create();
        return std::dynamic_pointer_cast<IApplication>(app);
#else
#error "Unknown Platform"
#endif
    }
}