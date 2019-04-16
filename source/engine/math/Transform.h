#pragma once

#include "math/Math.h"

namespace NTGS {
    class Transform {
    public:
        Transform();
        Transform(const Mat4d& mat);

        Vec3d operator()(const Vec3d& vec) const;

    private:
        Mat4d mMatrix;
        Mat4d mInvMatrix;
        
        bool mUpdateLater = false;
        Quaternion<double> mRotation;
        Vec3d mTranslate;
        Vec3d mScale;
    };
}