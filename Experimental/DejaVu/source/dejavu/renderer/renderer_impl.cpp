#include "renderer/renderer_impl.h"

namespace djv
{
    std::weak_ptr<RendererInterface> gpRenderer;

    std::shared_ptr<RendererInterface> createRenderer()
    {
        return nullptr;
    }
}