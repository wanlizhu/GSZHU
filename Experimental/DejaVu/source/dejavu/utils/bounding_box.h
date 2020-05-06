#pragma once

#include "utils/preprocess.h"
#include "utils/math3d.h"

namespace djv
{
    struct DJV_API BoundingBox
    {
        glm::vec3 center; // Center position of the bounding box
        glm::vec3 extent; // Essentially the coordinates to the max corner relative to the center

        static BoundingBox fromMinMax(const glm::vec3& min, const glm::vec3& max);
        static BoundingBox fromUnion(const BoundingBox& bb0, const BoundingBox& bb1);

        bool isValid() const;
        bool intersects(const BoundingBox& rhs) const;
        bool operator==(const BoundingBox& rhs) const;
        bool operator!=(const BoundingBox& rhs) const;

        glm::vec3 getMinPos() const;
        glm::vec3 getMaxPos() const;
        glm::vec3 getSize() const;
        float distanceTo(const glm::vec3& pos) const;
        float distanceTo(const BoundingBox& box) const;
        
        BoundingBox  transformTo(const glm::mat4& mat) const;
        BoundingBox  operator| (const BoundingBox& rhs) const;
        BoundingBox  operator& (const BoundingBox& rhs) const;
        BoundingBox& operator|=(const BoundingBox& rhs);
        BoundingBox& operator&=(const BoundingBox& rhs);  
    };
}