#include "utils/math3d.h"

namespace djv
{
    glm::quat quaternion_from_vectors(const glm::vec3& from, const glm::vec3& to)
    {
        glm::quat quat;
        glm::vec3 nFrom = glm::normalize(from);
        glm::vec3 nTo = glm::normalize(to);

        float dot = glm::dot(nFrom, nTo);
        dot = glm::clamp(dot, -1.0f, 1.0f);
        if (dot != 1)
        {
            float angle = acosf(dot);

            glm::vec3 cross = glm::cross(nFrom, nTo);
            glm::vec3 axis = glm::normalize(cross);

            quat = glm::angleAxis(angle, axis);
        }

        return quat;
    }

    glm::vec3 screen_to_world_ray(const glm::vec2& mousePos, const glm::mat4& viewMat, const glm::mat4& projMat)
    {
        // Convert from [0, 1] to [-1, 1] range
        const float x = mousePos.x * 2.0f - 1.0f;
        const float y = (1.0f - mousePos.y) * 2.0f - 1.0f;
        
        // NDC/Clip
        glm::vec4 ray(x, y, -1.0f, 1.0f);

        // View
        ray = glm::inverse(projMat) * ray;
        ray.z = -1.0f;
        ray.w = 0.0f;

        // World
        return glm::normalize(glm::inverse(viewMat) * ray);
    }

    glm::mat3 matrix_from_basis(const glm::vec3& forward, const glm::vec3& up)
    {
        glm::vec3 f = glm::normalize(forward);
        glm::vec3 s = glm::normalize(glm::cross(up, forward));
        glm::vec3 u = glm::cross(f, s);

        return glm::mat3(s, u, f);
    }

    glm::mat3 matrix_from_lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
    {
        return matrix_from_basis(target - pos, up);
    }

    glm::vec3 project_to_unit_sphere(glm::vec2 xy)
    {
        float xyLengthSquared = glm::dot(xy, xy);

        float z = 0;
        if (xyLengthSquared < 1)
        {
            z = sqrt(1 - xyLengthSquared);
        }
        else
        {
            xy = glm::normalize(xy);
        }
        return glm::vec3(xy.x, xy.y, z);
    }

    float focal_length_to_yfov(float focalLength, float frameHeight)
    {
        return 2.0f * atan(0.5f * frameHeight / focalLength);
    }

    float yfov_to_focal_length(float yfov, float frameHeight)
    {
        return frameHeight / (2.0f * tan(0.5f * yfov));
    }

    float aperture_F_to_radius(float f, float focalLength, float sceneUnit)
    {
        assert(f > 0.0f && focalLength > 0.f && sceneUnit > 0.f);
        float radius = 0.5f * focalLength / f; // in mm
        return radius * 0.001f / sceneUnit;
    }

    float aperture_radius_to_F(float radius_, float focalLength, float sceneUnit)
    {
        assert(focalLength > 0.f && sceneUnit > 0.f);
        float radius = radius_ * sceneUnit * 1000.f; // in mm
        return 0.5f * focalLength / radius;
    }

    float radical_inverse(uint32_t i)
    {
        i = (i & 0x55555555) << 1 | (i & 0xAAAAAAAA) >> 1;
        i = (i & 0x33333333) << 2 | (i & 0xCCCCCCCC) >> 2;
        i = (i & 0x0F0F0F0F) << 4 | (i & 0xF0F0F0F0) >> 4;
        i = (i & 0x00FF00FF) << 8 | (i & 0xFF00FF00) >> 8;
        i = (i << 16) | (i >> 16);
        return float(i) * 2.3283064365386963e-10f;
    }
}