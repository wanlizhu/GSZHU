#include "Configurations/Configuration.h"

namespace Wanli
{
    void Configuration::LoadConfig(const Path& path)
    {

    }

    void Configuration::SaveConfig(const Path& path)
    {

    }

    void Configuration::SealUp()
    {
        mSealed = true;
    }

    bool Configuration::IsSealed() const
    {
        return mSealed;
    }
}