#pragma once

#include "Core/IShape.h"

namespace NTGS {
    class Paraboloid : public IShape {
    public:
        static std::shared_ptr<Paraboloid> Create();

    protected:
        FLOAT mRadius = 0;
        FLOAT mMinZ = 0;
        FLOAT mMaxZ = 0;
        FLOAT mMaxPhi = 0;
    };
}