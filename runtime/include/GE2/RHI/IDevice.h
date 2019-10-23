#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "IDeviceObject.h"
#include "EDeviceFeature.h"

namespace GE2::RHI
{
    enum class EDeviceType
    {
        D3D12,
        Vulkan,
    };

    class GE2_IMPEXP IDevice : public IDeviceObject
    {
    public:
        struct InitializeData : public IDeviceObject::InitializeData
        {
            STRLIST              instanceLayers;
            STRLIST              instanceExtensions;

            STRLIST              deviceExtensions;
            LIST<EDeviceFeature> deviceFeatures;
        };

        IDevice() = default; // for std::make_shared<>() to call
        IDevice(const IDevice&) = delete;
        IDevice(IDevice&&) = delete;
        IDevice& operator=(const IDevice&) = delete;
        IDevice& operator=(IDevice&&) = delete;
        virtual ~IDevice() {};

        // IDeviceObject's abstract virtual methods
        virtual bool   Initialize(const InitializeData& data) = 0;
        virtual void   Destroy() = 0;
        virtual CSTR   GetName() const = 0;
        virtual HANDLE GetHandle() const = 0;
    };

    GE2_IMPEXP SPTR<IDevice> CreateDevice();
}