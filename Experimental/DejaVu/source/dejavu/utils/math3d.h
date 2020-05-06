#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils/preprocess.h"

namespace djv
{
    glm::quat DJV_API quaternion_from_vectors(const glm::vec3& from, const glm::vec3& to);
    glm::vec3 DJV_API screen_to_world_ray(const glm::vec2& mousePos, const glm::mat4& viewMat, const glm::mat4& projMat);
    glm::mat3 DJV_API matrix_from_basis(const glm::vec3& forward, const glm::vec3& up);
    glm::mat3 DJV_API matrix_from_lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);
    glm::vec3 DJV_API project_to_unit_sphere(glm::vec2 xy);
    float     DJV_API focal_length_to_yfov(float focalLength, float frameHeight);
    float     DJV_API yfov_to_focal_length(float yfov, float frameHeight);
    float     DJV_API aperture_F_to_radius(float f, float focalLength, float sceneUnit);
    float     DJV_API aperture_radius_to_F(float radius, float focalLength, float sceneUnit);
    float     DJV_API radical_inverse(uint32_t i); // Base 2 Van der Corput radical inverse
}