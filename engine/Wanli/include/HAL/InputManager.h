#pragma once

#include "Utilities/NonCopyable.h"
#include "BasicTypes.h"

namespace Wanli
{
    class DLLDECL InputManager : public NonCopyable
    {
    public:
        static InputManager* Get();

    protected:
        InputManager();
        virtual ~InputManager();
    };
}
