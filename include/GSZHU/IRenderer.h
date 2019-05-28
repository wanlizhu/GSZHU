#pragma once

#include <GSZHU/CBuilderBase.h>
#include <GSZHU/Config.h>
#include <GSZHU/IUserInput.h>

namespace GSZHU {
    // Note: double inheritance of enable_shared_from_this<> is dangerous 
    class IRenderer : public IUserInput {
    public:

    };
}