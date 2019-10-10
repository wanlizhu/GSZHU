#include "GE2/CGA/internal/Distance3D_LineToAlignedBox.h"

namespace CGA
{
    void ComputeDistance3D(const Line& line, const AlignedBox& box, LineToAlignedBox* result)
    {
        // Translate the line and box so that the box has center at the origin of coordinate system.
        Vec3 boxCenter = (box.max() + box.min()) * 0.5f;
        Vec3 boxExtent = (box.max() - box.min()) * 0.5f;
        Vec3 linePoint = line.GetOrigin() - boxCenter;
        Vec3 lineVec   = line.GetDirection;

        result->distance = 0.f;
        result->squaredDistance = 0.f;
        result->lineOffset = 0.f;

        // Apply reflections so that direction vector has nonnegative components.
        bool reflect[3] = { false };
        for (int i = 0; i < 3; ++i)
        {
            if (lineVec[i] < 0.f)
            {
                linePoint[i] = -linePoint[i];
                lineVec[i] = -lineVec[i];
                reflect[i] = true;
            }
        }

        result->closestPoint[0] = line.PointAt(result->lineOffset);
        result->closestPoint[1] = boxCenter + linePoint;
    }
}