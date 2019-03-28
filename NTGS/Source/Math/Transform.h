#pragma once

#include "Math/Math.h"

namespace NTGS {
    class Transform {
    public:


    private:
        Mat4f mMatrix;
        Mat4f mInvMatrix;
        
        bool mUpdateLater = false;
        Quaternion<double> mRotation;
        Vec3d mTranslate;
        Vec3d mScale;
    };
}