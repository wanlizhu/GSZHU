#pragma once

#include "Common/Config.h"
#include "Graphics/Object.h"

namespace ZHU
{
    struct ZHU_API DeviceObjectDescriptor : public Object
    {
        std::string Name = "";

        DeviceObjectDescriptor(const std::string& name);
        
        void AddDependency(const std::string& name);
        size_t GetDependencyCount() const;
        const std::string& GetDependency(size_t index) const;

    private:
        std::vector<std::string> mObjectDependencies;
    };
}