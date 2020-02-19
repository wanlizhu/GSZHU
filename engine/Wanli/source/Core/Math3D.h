#pragma once

#include "Core/BasicTypes.h"

namespace Wanli
{
    glm::quat Quaternion(const glm::vec3& from, const glm::vec3& to);
    glm::quat Quaternion(const glm::vec3& eulerAngles);
    glm::quat Quaternion(const glm::vec3& axis, float radius);
    
    void ApplyTo(glm::quat& target, const glm::quat& value);
    void ApplyTo(glm::vec3& pos, const glm::quat& rotation);
    void ApplyTo(glm::mat4& matrix, const glm::quat& rotation);
}