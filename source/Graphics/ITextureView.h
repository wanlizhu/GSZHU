#pragma once

#include "IDeviceObject.h"
#include "CTextureViewDesc.h"
#include "ISampler.h"

namespace GSZHU {
    class ITextureView : public IDeviceObject {
    public:
        virtual CTextureViewDesc const& GetDesc() const = 0;
        virtual void SetSampler(std::shared_ptr<ISampler> sampler) = 0;
        virtual std::shared_ptr<ISampler> GetSampler() const = 0;
        virtual std::shared_ptr<ITexture> GetTexture() const = 0;
    };
}