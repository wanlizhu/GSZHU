#pragma once

#include "Core/IShape.h"

namespace NTGS {
    class Sphere : public IShape {
    public:
        static std::shared_ptr<Sphere> Create();

    protected:
        FLOAT mRadius = 0;
        FLOAT mMinZ = 0;
        FLOAT mMaxZ = 0;
        FLOAT mMinTheta = 0;
        FLOAT mMaxTheta = 0;
        FLOAT mMaxPhi = 0;
    };
}