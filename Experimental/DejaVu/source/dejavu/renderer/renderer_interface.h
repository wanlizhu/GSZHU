#pragma once

#include "utils/preprocess.h"

namespace djv
{
    class DJV_API RendererInterface : public std::enable_shared_from_this<RendererInterface>
    {
    public:

    };

    DJV_API extern std::weak_ptr<RendererInterface> gpRenderer;

    std::shared_ptr<RendererInterface> DJV_API createRenderer();
}