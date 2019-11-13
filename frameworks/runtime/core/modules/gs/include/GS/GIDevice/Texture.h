#pragma once

#include "ResourcePool.h"

namespace GS::GI
{
    class Texture : public ResourceNoWait
    {
    public:
        using Ptr = std::shared_ptr<Texture>;

        Texture(const char* name,
                Device::Ptr device,
                Size size,
                const void* data = nullptr);

        // virtual methods from `ResourceNoWait`
        virtual void Flush() override;
        virtual void Init() override;
        virtual void Destroy() override;
        virtual operator bool() const override;

    protected:

    };
}