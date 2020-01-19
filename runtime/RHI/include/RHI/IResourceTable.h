#pragma once

#include "RHI/IResource.h"

namespace Wanlix
{
    class IResourceTable
    {
    public:
        virtual void AddResource(const String& name, IResource* res) = 0;
        virtual void AddResourceArray(const String& name, const Array<IResource*> array) = 0;
        virtual void RemoveResource(const String& name, Uint index = 0) = 0;
        virtual IResource* GetResource(const String& name, Uint index = 0) = 0;
        virtual Uint GetSize() const = 0;
        virtual void Clear() = 0;
    };
}