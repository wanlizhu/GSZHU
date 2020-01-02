#pragma once

#include "Wanlix/RHI/Common.h"

namespace Wanlix
{
    class RHI_API IAllocator : public Object
    {
        CLASSINFO(IAllocator)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual void* Allocate(Size size, String const& dbgDesc) = 0;
        virtual void  Free(void* ptr) = 0;
    };
}