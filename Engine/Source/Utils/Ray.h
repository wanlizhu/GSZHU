#pragma once

#include "Core/BasicTypes.h"
#include "Utils/NonCopyable.h"

namespace Wanli
{
    class Ray : public NonCopyable
    {
    public:

    protected:
        bool mUseMouse = true;
        glm::vec2 mScreenPos;

        glm::mat4 mViewMatrix;
        glm::mat4 mProjMatrix;

        glm::vec2 mNormalizedPos;
        glm::vec4 mClipPos;
        glm::vec4 mEyePos;

        glm::mat4 mViewMatrixInv;
        glm::mat4 mProjMatrixInv;
        glm::vec4 mRayWorld;

        glm::vec3 mOrigin;
        glm::vec3 mCurrentRay;
    };
}