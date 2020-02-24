#include "Scenes/Helpers/Frustum.h"

namespace Wanli
{
    void Frustum::Update(const glm::mat4& view, const glm::mat4& proj)
    {

    }

    bool Frustum::CheckPoint(const glm::vec3& pos) const
    {
        return false;
    }

    bool Frustum::CheckSphere(const glm::vec3& pos, float radius) const
    {
        return false;
    }

    bool Frustum::CheckCube(const glm::vec3& min, const glm::vec3& max) const
    {
        return false;
    }

    void Frustum::NormalizePlane(int side)
    {

    }
}