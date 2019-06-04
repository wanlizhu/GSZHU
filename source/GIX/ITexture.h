#pragma once

#include "IDeviceObject.h"
#include "CTextureDesc.h"
#include "ITextureView.h"

namespace GSZHU {
    class ITexture : public IDeviceObject {
    public:
        virtual CTextureDesc const& GetDesc() const = 0;
        virtual std::shared_ptr<ITextureView> CreateView(CTextureViewDesc const& desc) = 0;
        virtual void* GetNativeHandle() const = 0;
    };
}