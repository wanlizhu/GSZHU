#pragma once

#include "Core/IShape.h"

namespace NTGS {
    class Hyperboloid : public IShape {
    public:
        static std::shared_ptr<Hyperboloid> Create();

    protected:
        Vec3 mP1, mP2;
        FLOAT mMinZ = 0;
        FLOAT mMaxZ = 0;
        FLOAT mMaxPhi = 0;
        FLOAT mMaxR = 0;
        FLOAT mAH = 0, mCH = 0;
    };
}