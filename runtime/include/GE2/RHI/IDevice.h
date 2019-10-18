#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"

namespace GE2::RHI
{
    enum class EDeviceType
    {
        Default,
        D3D12,
        Vulkan,
    };

    struct InitializeData 
    {
        STRLIST instanceExtensions;
        STRLIST enableLayers;
        STRLIST deviceExtensions;
    };

    class GE2_IMPEXP IDevice : public std::enable_shared_from_this<IDevice>
    {
    public:
        using SharedPtr = std::shared_ptr<IDevice>;
        using WeakPtr = std::weak_ptr<IDevice>;

        IDevice() = default;
        IDevice(const IDevice&) = delete;
        IDevice(IDevice&&) = delete;
        IDevice& operator=(const IDevice&) = delete;
        IDevice& operator=(IDevice&&) = delete;

        virtual ~IDevice() {};
        virtual bool Initialize(const InitializeData& data) = 0;
        virtual void Destroy() = 0;

    };

    IDevice::SharedPtr CreateDevice();
}