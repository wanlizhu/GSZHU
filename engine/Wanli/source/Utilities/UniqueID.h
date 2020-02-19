#pragma once

#include "Core/BasicTypes.h"

namespace Wanli
{
    Uint DLLDECL GetNextIdFor(std::type_index type);
    
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