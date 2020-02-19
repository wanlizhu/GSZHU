#include "Configurations/IConfig.h"
#include "Configurations/Configuration.h"

namespace Wanli
{
    void IConfig::AssertWritable()
    {
        if (mConfigManager.IsSealed())
        {
            throw std::runtime_error("Sealed configuration object is not writable");
        }
    }
}