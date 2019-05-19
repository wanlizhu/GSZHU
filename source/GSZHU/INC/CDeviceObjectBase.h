#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/CUniqueID.h>

namespace GSZHU {
    template<typename INTERFACE, typename RENDER_DEVICE>
    class CDeviceObjectBase : public INTERFACE {
    public:
        using BASE = INTERFACE;
        using OBJECT_DESC = typename INTERFACE::DESC;

        CDeviceObjectBase(const DeviceObjectBase&) = delete;
        CDeviceObjectBase(CDeviceObjectBase&&) = delete;
        CDeviceObjectBase& operator=(const CDeviceObjectBase&) = delete;
        CDeviceObjectBase& operator=(CDeviceObjectBase&&) = delete;
        CDeviceObjectBase(RENDER_DEVICE* Device, const OBJECT_DESC& Desc)
            : mDevice(Device)
            , mDesc(Desc)
            , mObjectName(Desc.Name ? STRING(Desc.Name) : GetAddress()) {
            mDesc.Name = mObjectName.c_str();
            // We cannot add resource to the hash table from here, 
            // because the object has not been completely created yet 
            //m_pDevice->AddResourceToHash( this ); - ERROR!
        }
        virtual ~CDeviceObjectBase() {}

        virtual const OBJECT_DESC& GetDesc() const override final {
            return mDesc;
        }

        ATOMIC_LONG GetUniqueID() const noexcept {
            return mUniqueID.GetID();
        }

        RENDER_DEVICE* GetDevice() const noexcept {
            return mDevice;
        }

    private:
        STRING GetAddress() const noexcept { 
            std::stringstream ss;
            ss << this;
            return ss.str();
        }

    protected:
        RENDER_DEVICE* mDevice = nullptr;
        OBJECT_DESC mDesc;
        STRING mObjectName; // mDesc.Name is set to mObjectName.c_str()
        CUNIQUE_ID<INTERFACE> mUniqueID; // <INTERFACE> is only used to separate counters for different groups of objects
    };
}