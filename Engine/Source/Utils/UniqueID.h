#pragma once

#include "Core/Common.h"

namespace Wanli
{
    Uint WANLI_API GetNextIdFor(std::type_index type);
    
    template<typename T>
    class UniqueID
    {
    public:
        static inline Uint Get() noexcept
        {
            return GetNextIdFor(typeid(T));
        }
    };
}