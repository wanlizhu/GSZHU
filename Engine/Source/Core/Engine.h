#pragma once

#include "WanliConfig.h"
#include "Common.h"
#include "IRenderer.h"
#include "Utils/NonCopyable.h"

namespace Wanli
{
    class Engine : public NonCopyable
    {
    public:
        static WANLI_API Engine* Get();

    private:
    
    };
}