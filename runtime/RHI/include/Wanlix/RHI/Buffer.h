#pragma once

#include "Resource.h"
#include "BufferDescriptor.h"

namespace Wanlix
{
    class Buffer : public Resource
    {
    public:
        using SharedPtr = std::shared_ptr<Buffer>;
        using UniquePtr = std::unique_ptr<Buffer>;
        using WeakPtr   = std::weak_ptr<Buffer>;
        using Desc      = BufferDescriptor;

        virtual const Desc& GetDescriptor() const = 0;

    protected:
        Buffer(
            const std::string& name,
            std::shared_ptr<Device> device
        ) noexcept;
    };
}