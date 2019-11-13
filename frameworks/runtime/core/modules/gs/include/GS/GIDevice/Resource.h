#pragma once

#include "DeviceObject.h"
#include "EResourceState.h"
#include "ResourcePool.h"
#include "Common.h"

namespace GS::GI
{
    enum class EResourceType
    {
        Undefined = 0,
        Buffer,
        Texture1D,
        Texture2D,
        Texture3D,
        TextureCube,
        Texture2DMS, // 2D multi-sampled texture
    };

    class IResource
    {
    public:
        using Ptr = std::shared_ptr<IResource>;

        virtual bool IsHostResource() const = 0;
    };

    class HostResource : public IResource 
    {
    public:
        using Ptr = std::shared_ptr<HostResource>;

        HostResource() = default;
        HostResource(void* _data, size_t _size)
            : data(_data)
            , constData(_data)
            , size(_size)
        {}
        HostResource(const void* _data, size_t _size)
            : data(nullptr)
            , constData(_data)
            , size(_size)
        {}

        virtual bool IsHostResource() const override { return true; }
        bool IsEmpty() const { return size == 0 || constData == nullptr; }
        bool IsConst() const { return constData && !data; }

    public:
        void* data = nullptr;
        const void* constData = nullptr;
        size_t size = 0;
    };

    class DeviceResource : public DeviceObject
                         , public IResource
    {
        friend class ResourcePool;
    public:
        using Ptr = std::shared_ptr<DeviceResource>;

        virtual bool IsHostResource() const override { return false; }
        virtual void Flush();
        virtual void UpdateRegion(const Region& region, const void* data);
        virtual void Init() override;
        virtual void Destroy() override;
        virtual operator bool() const override;

        inline  EResourceType  GetType() const { return mType; }
        virtual EResourceState GetState() const;
        virtual void SetState(EResourceState state);

    protected:
        DeviceResource(const char* name,
                       Device::Ptr device,
                       EResourceType type,
                       EResourceState state);

    protected:
        EResourceType  mType = EResourceType::Undefined;
        EResourceState mState = EResourceState::Undefined;

        ResourcePool::Ptr mPool;
        Range mRangeInPool;
    };

    // How-to-implement-NoWait shall be invisible to the derived class
    class ResourceNoWait : public DeviceResource
    {
    protected:
        using Ptr = std::shared_ptr<ResourceNoWait>;

        ResourceNoWait(const char* name, 
                       Device::Ptr device,
                       EResourceType type,
                       EResourceState state);

        virtual void Flush() override;
        virtual void UpdateRegion(const Region& region, const void* data);
        virtual void Init() override;
        virtual void Destroy() override;

    private: 

    private:

    };
}