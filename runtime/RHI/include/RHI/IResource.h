#pragma once

#include "RHI/IDeviceObject.h"

namespace Wanlix
{
    class IResource : public IDeviceObject
    {
    public:
        virtual EResourceType GetType() const = 0;
    };
}