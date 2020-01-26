#pragma once

#include "Core/Config.h"
#include "Wanlix/Common.h"

namespace Wanlix
{
    struct BoundingBox
    {
        Float3 center;
        Float3 extent;

        static BoundingBox FromMinMax(const Float3& min, const Float3& max) {
            BoundingBox box;
            box.center = (max + min) * Float3(0.5f);
            box.extent = (max - min) * Float3(0.5f);
            return box;
        }

        static BoundingBox Union(const BoundingBox& box0, const BoundingBox& box1) {
            return BoundingBox::FromMinMax(glm::min(box0.GetMinPos(), box1.GetMinPos()),
                                           glm::max(box0.GetMaxPos(), box1.GetMaxPos()));
        }

        FORCEINLINE bool operator==(const BoundingBox& rhs) const {
            return center == rhs.center && extent == rhs.extent;
        }

        FORCEINLINE Float3 GetMinPos() const {
            return center - extent;
        }

        FORCEINLINE Float3 GetMaxPos() const {
            return center + extent;
        }

        FORCEINLINE Float3 GetSize() const {
            return extent * 2.0f;
        }

        FORCEINLINE BoundingBox transform(const Float4x4& mat) const {
            Float3 min = center - extent;
            Float3 max = center + extent;

            Float3 xa = glm::vec3(mat[0] * min.x);
            Float3 xb = glm::vec3(mat[0] * max.x);
            Float3 xMin = glm::min(xa, xb);
            Float3 xMax = glm::max(xa, xb);

            Float3 ya = glm::vec3(mat[1] * min.y);
            Float3 yb = glm::vec3(mat[1] * max.y);
            Float3 yMin = glm::min(ya, yb);
            Float3 yMax = glm::max(ya, yb);

            Float3 za = glm::vec3(mat[2] * min.z);
            Float3 zb = glm::vec3(mat[2] * max.z);
            Float3 zMin = glm::min(za, zb);
            Float3 zMax = glm::max(za, zb);


            Float3 newMin = xMin + yMin + zMin + glm::vec3(mat[3]);
            Float3 newMax = xMax + yMax + zMax + glm::vec3(mat[3]);

            return BoundingBox::FromMinMax(newMin, newMax);
        }
    };
}