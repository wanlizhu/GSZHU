#pragma once

#include "RHI/IDeviceObject.h"

namespace Wanlix
{
    class IShader : public IDeviceObject
    {
    public:
        virtual bool Compile(ByteArrayPtr blob,
                             const String& entryPoint,
                             EShaderCompileFlags compileFlags,
                             const StringMap<String>& defines = {}) = 0;
        virtual bool IsCompiled() const = 0;
        virtual EShaderType GetType() const = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
    };
}