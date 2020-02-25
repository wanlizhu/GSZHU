#pragma once
#ifdef PLATFORM_WINDOWS

#include "Configuration/IConfig.h"
#include "Utils/ArgList.h"

namespace Wanli
{
    class WANLI_API EngineConfig : public IConfig::Registrar<EngineConfig>
    {
        DECL_DATA_FIELD(ArgList, ArgList, {});
        DECL_DATA_FIELD(EnableRenderThread, bool, false);
        DECL_DATA_FIELD(PrioritizeUpdateOverRender, bool, false);
        DECL_DATA_FIELD(FPSLimit, Optional<float>, std::nullopt);
        DECL_DATA_FIELD(UPSLimit, Optional<float>, std::nullopt);
    };
}

#endif