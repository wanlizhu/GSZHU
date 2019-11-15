#pragma once

#include "GIResource.h"

namespace GS::GI
{
    class GITexture : public GIResource
    {
    public:
        using Ptr = std::shared_ptr<GITexture>;
        using ConstPtr = std::shared_ptr<const GITexture>;

        static Ptr Create(const std::string& name,
                          GIDevice::Ptr device,
                          GISize size,
                          const void* data = nullptr);

        virtual void Flush() override;
        virtual void Init(void* data) override;
        virtual void Destroy() override;

    protected:
        GITexture(const std::string& name,
                  GIDevice::Ptr device,
                  GISize size,
                  const void* data = nullptr);
    };
}