#pragma once

#include "Core/BasicTypes.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class Configuration;

    class DLLDECL IConfig : public NonCopyable
    {
    public:
        IConfig(const Configuration& config)
            : mConfigManager(config)
        {}
        virtual ~IConfig() = default;
        
    protected:
        void AssertWritable();

    private:
        const Configuration& mConfigManager;
    };
}