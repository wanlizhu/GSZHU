#include "Wanlix/Core3D/Context.h"

namespace Wanlix
{

    Context::Ptr Context::Create() {
        return std::shared_ptr<Context>(new Context());
    }

    bool Context::Initialize() {
        assert(!weak_from_this().expired());

        bool result = true;
        for (const auto& mod : mModules) {
            if (!mod->Initialize()) {
                result = false;
            }
        }
        return result;
    }

    void Context::Tick(float delta)
    {
        for (const auto& mod : mModules) {
            mod->Tick(delta);
        }
    }
}