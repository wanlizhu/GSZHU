#pragma once

#include <GSZHU/DESC/SBufferDesc.h>
#include <GSZHU/DESC/SBufferViewDesc.h>
#include <GSZHU/IDeviceObject.h>
#include <GSZHU/ENUM/EResourceState.h>

namespace GSZHU {
    class IBufferView;

    class IBuffer : public IDeviceObject {
    public:
        using DESC = SBufferDesc;

        virtual const SBufferDesc& GetDesc() const = 0;
        virtual IBufferView* CreateView(const SBufferViewDesc& ViewDesc) = 0;
        virtual IBufferView* GetDefaultView(EBUFFER_VIEW_TYPE ViewType) = 0;
        virtual void* GetNativeHandle() = 0;
        virtual void SetState(ERESOURCE_STATE State) = 0;
        virtual ERESOURCE_STATE GetState() const = 0;
    };
}