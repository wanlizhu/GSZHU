#pragma once

#include <Eigen/Eigen>

namespace Wanlix
{
    using float4x4  = Eigen::Matrix4f;
    using float4x4d = Eigen::Matrix4d;

    using float3x3  = Eigen::Matrix3f;
    using float3x3d = Eigen::Matrix3d;
    
    using float4    = Eigen::Vector4f;
    using float4d   = Eigen::Vector4d;
    using float3    = Eigen::Vector3f;
    using float3d   = Eigen::Vector3d;
    using float2    = Eigen::Vector2f;
    using float2d   = Eigen::Vector2d;

}