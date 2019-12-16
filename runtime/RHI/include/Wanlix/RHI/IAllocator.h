#pragma once

#include "Wanlix/Object.h"

namespace Wanlix
{
    class IAllocator : public inherit_shared_from_this<Object, IAllocator>
    {
    public:
        virtual void* Allocate(Size size, String const& dbgDesc) = 0;
        virtual void  Free(void* ptr) = 0;
    };
}