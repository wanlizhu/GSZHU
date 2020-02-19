#include "Configurations/InputConfig.h"
#include "Application/IApplication.h"

namespace Wanli
{
    InputConfig& InputConfig::SetApp(IApplication* app)
    {
        AssertWritable();
        mApp = app;
        return *this;
    }
}