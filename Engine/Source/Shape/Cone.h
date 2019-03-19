#pragma once

#include "Core/IShape.h"

namespace NTGS {
    class Cone : public IShape {
    public:
        static std::shared_ptr<Cone> Create();

    protected:
        FLOAT mRadius = 0;
        FLOAT mHeight = 0;
        FLOAT mMaxPhi = 0;
    };
}