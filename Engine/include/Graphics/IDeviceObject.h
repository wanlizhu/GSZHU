#pragma once

#include "Common/Config.h"
#include "Graphics/Object.h"
#include "Graphics/Constants.h"
#include "Graphics/Tools.h"
#include "Graphics/DeviceObjectDescriptor.h"

namespace ZHU
{
    class ZHU_API IDeviceObject : public Object
    {
    public:
        using Pointer = std::shared_ptr<IDeviceObject>;

        IDeviceObject(const std::string& name);
        virtual const DeviceObjectDescriptor& GetDesc() const = 0;
        
        static Pointer FindDeviceObject(const std::string& name);
        static size_t GetDeviceObjectCount();
        static std::weak_ptr<IDeviceObject> GetDeviceObject(size_t index);

    protected:
        virtual bool Initialize() = 0;

    private:
        static std::unordered_map<std::string, std::weak_ptr<IDeviceObject>> smDeviceObjects;
    };
}