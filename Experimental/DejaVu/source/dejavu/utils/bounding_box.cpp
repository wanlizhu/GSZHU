#include "utils/bounding_box.h"

namespace djv
{
    BoundingBox BoundingBox::fromMinMax(const glm::vec3& min, const glm::vec3& max)
    {
        BoundingBox box;
        box.center = (max + min) * glm::vec3(0.5f);
        box.extent = (max - min) * glm::vec3(0.5f);
        return box;
    }

    BoundingBox BoundingBox::fromUnion(const BoundingBox& bb0, const BoundingBox& bb1)
    {
        const auto& minPos = glm::min(bb0.getMinPos(), bb1.getMinPos());
        const auto& maxPos = glm::max(bb0.getMaxPos(), bb1.getMaxPos());
        return BoundingBox::fromMinMax(minPos, maxPos);
    }

    bool BoundingBox::isValid() const
    {
        const glm::vec3& minPos = center - extent;
        const glm::vec3& maxPos = center + extent;
        return glm::all(glm::lessThan(minPos, maxPos));
    }

    bool BoundingBox::intersects(const BoundingBox& rhs) const
    {
        const glm::vec3& minPos = getMinPos();
        const glm::vec3& maxPos = getMaxPos();

        return glm::all(glm::greaterThanEqual(maxPos, rhs.getMinPos())) 
            && glm::all(glm::lessThanEqual(minPos, rhs.getMaxPos()));
    }

    bool BoundingBox::operator==(const BoundingBox& rhs) const
    {
        return (rhs.center == center) && (rhs.extent == extent);
    }

    bool BoundingBox::operator!=(const BoundingBox& rhs) const
    {
        return !(*this == rhs);
    }

    glm::vec3 BoundingBox::getMinPos() const
    {
        return center - extent;
    }

    glm::vec3 BoundingBox::getMaxPos() const
    {
        return center + extent;
    }

    glm::vec3 BoundingBox::getSize() const
    {
        return extent * 2.0f;
    }

    float BoundingBox::distanceTo(const glm::vec3& pos) const
    {
        glm::vec3 d1 = getMinPos() - pos;
        glm::vec3 d2 = pos - getMaxPos();
        glm::vec3 d = glm::max(glm::max(d1, d2), 0.f);
        return glm::length(d);
    }

    float BoundingBox::distanceTo(const BoundingBox& box) const
    {
        glm::vec3 d1 = getMinPos() - box.getMaxPos();
        glm::vec3 d2 = box.getMinPos() - getMaxPos();
        glm::vec3 d = glm::max(glm::max(d1, d2), 0.f);
        return glm::length(d);
    }

    BoundingBox BoundingBox::transformTo(const glm::mat4& mat) const
    {
        glm::vec3 min = center - extent;
        glm::vec3 max = center + extent;

        glm::vec3 xa = glm::vec3(mat[0] * min.x);
        glm::vec3 xb = glm::vec3(mat[0] * max.x);
        glm::vec3 xMin = glm::min(xa, xb);
        glm::vec3 xMax = glm::max(xa, xb);

        glm::vec3 ya = glm::vec3(mat[1] * min.y);
        glm::vec3 yb = glm::vec3(mat[1] * max.y);
        glm::vec3 yMin = glm::min(ya, yb);
        glm::vec3 yMax = glm::max(ya, yb);

        glm::vec3 za = glm::vec3(mat[2] * min.z);
        glm::vec3 zb = glm::vec3(mat[2] * max.z);
        glm::vec3 zMin = glm::min(za, zb);
        glm::vec3 zMax = glm::max(za, zb);


        glm::vec3 newMin = xMin + yMin + zMin + glm::vec3(mat[3]);
        glm::vec3 newMax = xMax + yMax + zMax + glm::vec3(mat[3]);

        return BoundingBox::fromMinMax(newMin, newMax);
    }

    BoundingBox BoundingBox::operator| (const BoundingBox& rhs) const
    {
        return BoundingBox(*this) |= rhs;
    }

    BoundingBox BoundingBox::operator& (const BoundingBox& rhs) const
    {
        return BoundingBox(*this) &= rhs;
    }

    BoundingBox& BoundingBox::operator|=(const BoundingBox& rhs)
    {
        glm::vec3 minPos = getMinPos();
        glm::vec3 maxPos = getMaxPos();
        minPos = glm::min(minPos, rhs.getMinPos());
        maxPos = glm::max(maxPos, rhs.getMaxPos());

        *this = fromMinMax(minPos, maxPos);
        return *this;
    }

    BoundingBox& BoundingBox::operator&=(const BoundingBox& rhs)
    {
        glm::vec3 minPos = getMinPos();
        glm::vec3 maxPos = getMaxPos();
        minPos = glm::max(minPos, rhs.getMinPos());
        maxPos = glm::min(maxPos, rhs.getMaxPos());

        *this = fromMinMax(minPos, maxPos);
        return *this;
    }
}