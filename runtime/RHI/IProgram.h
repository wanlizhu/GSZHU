#pragma once

#include "RHI/IShader.h"

namespace Wanlix
{
    class IProgram : public IDeviceObject
    {
    public:
        

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
    };
}