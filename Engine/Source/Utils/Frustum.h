#pragma once

#include "Core/BasicTypes.h"
#include "Utils/NonCopyable.h"

namespace Wanli
{
    class Frustum : public NonCopyable
    {
    public:
        void Update(const glm::mat4& view, const glm::mat4& proj);
        
        bool CheckPoint(const glm::vec3& pos) const;
        bool CheckSphere(const glm::vec3& pos, float radius) const;
        bool CheckCube(const glm::vec3& min, const glm::vec3& max) const;

    protected:
        void NormalizePlane(int side);

    protected:
        std::array<std::array<float, 4>, 6> mFrustum = {};
    };
}