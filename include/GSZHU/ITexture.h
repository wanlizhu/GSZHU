#pragma once

#include <GSZHU/STextureData.h>
#include <GSZHU/IDeviceObject.h>
#include <GSZHU/DESC/STextureDesc.h>
#include <GSZHU/ITextureView.h>
#include <GSZHU/ENUM/EResourceState.h>

namespace GSZHU {
    class ITexture : public IDeviceObject {
    public:
        using DESC = STextureDesc;

        virtual const STextureDesc& GetDesc()const = 0;
        virtual ITextureView* CreateView(const STextureViewDesc& ViewDesc) = 0;
        virtual ITextureView* GetDefaultView(ETEXTURE_VIEW_TYPE ViewType) = 0;
        virtual void* GetNativeHandle() = 0;
        virtual void SetState(ERESOURCE_STATE State) = 0;
        virtual ERESOURCE_STATE GetState() const = 0;
    };
}