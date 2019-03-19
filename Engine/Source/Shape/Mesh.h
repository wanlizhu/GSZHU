#pragma once

#include "Core/IShape.h"

namespace NTGS {
    class Mesh : public IShape {
    public:
        static std::shared_ptr<Mesh> Create();

    protected:
        
    };
}