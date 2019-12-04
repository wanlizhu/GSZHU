#pragma once

#include "Resource.h"
#include "TextureDescriptor.h"

namespace Wanlix
{
    class Texture : public Resource
    {
    public:
        using SharedPtr = std::shared_ptr<Texture>;
        using UniquePtr = std::unique_ptr<Texture>;
        using WeakPtr   = std::weak_ptr<Texture>;
        using Desc      = TextureDescriptor;

        virtual const Desc& GetDescriptor() const = 0;

    protected:
        Texture(
            const std::string& name,
            std::shared_ptr<Device> device
        ) noexcept;
    };
}