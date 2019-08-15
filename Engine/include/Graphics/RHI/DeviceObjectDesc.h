#pragma once

#include "Graphics/Config.h"
#include "Graphics/RHI/Object.h"

namespace ZHU
{
    struct ZHU_GS_API DeviceObjectDesc : public Object
    {
        DeviceObjectDesc(const std::string& name);
        
        void AddDependency(const std::string& name);
        size_t GetDependencyCount() const;
        const std::string& GetDependency(size_t index) const;

    private:
        std::vector<std::string> mObjectDependencies;
    };
}