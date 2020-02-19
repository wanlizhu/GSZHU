#pragma once

#include "Core/BasicTypes.h"
#include "Utilities/Delegate.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    class DLLDECL IRenderer : virtual Observer
        , public NonCopyable
        , public std::enable_shared_from_this<IRenderer>
    {
    public:
        virtual ~IRenderer() = default;
        virtual bool Initialize() = 0;
        virtual void Update() = 0;
    };
}