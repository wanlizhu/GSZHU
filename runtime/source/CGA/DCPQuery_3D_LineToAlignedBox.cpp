#include "GE2/CGA/internal/DCPQuery_3D_LineToAlignedBox.h"

namespace CGA
{
    void Face(int i0, int i1, int i2, 
              Point& linePoint, const Vector& lineVec, 
              const Vector& PmE, const Vector& boxExtent,
              DCPQuery<Line, AlignedBox>::ComputeResult* result);

    void CaseNoZero(Point& linePoint, const Vector& lineVec, 
                    const Vector& boxExtent,
                    DCPQuery<Line, AlignedBox>::ComputeResult* result);

    void Case0(int i0, int i1, int i2,
               Point& linePoint, const Vector& lineVec, 
               const Vector& boxExtent,
               DCPQuery<Line, AlignedBox>::ComputeResult* result);

    void Case00(int i0, int i1, int i2,
                Point& linePoint, const Vector& lineVec, 
                const Vector& boxExtent,
                DCPQuery<Line, AlignedBox>::ComputeResult* result);

    void Case000(Point& linePoint,
                 const Vector& boxExtent,
                 DCPQuery<Line, AlignedBox>::ComputeResult* result);

    DCPQuery<Line, AlignedBox>::ComputeResult 
        DCPQuery<Line, AlignedBox>::operator()(const Line& line, const AlignedBox& box)
    {
        ComputeResult result;
        ComputeDistance3D(line, box, &result);
        return result;
    }

    void DCPQuery<Line, AlignedBox>::ComputeDistance3D(const Line& line, 
                                                       const AlignedBox& box, 
                                                       DCPQuery<Line, AlignedBox>::ComputeResult* result)
    {
        // Translate the line and box so that the box has center at the origin of coordinate system.
        Point  boxCenter = (box.max() + box.min()) * 0.5;
        Vector boxExtent = (box.max() - box.min()) * 0.5;
        Point  linePoint = line.GetOrigin() - boxCenter;
        Vector lineVec   = line.GetDirection();

        result->distance = 0.0;
        result->squaredDistance = 0.0;
        result->lineParameter = 0.0;

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

        if (lineVec[0] > 0.0)
        {
            if (lineVec[1] > 0.0)
            {
                if (lineVec[2] > 0.0) // (+,+,+)
                    CaseNoZero(linePoint, lineVec, boxExtent, result);
                else // (+,+,0)
                    Case0(0, 1, 2, linePoint, lineVec, boxExtent, result);
            }
            else
            {
                if (lineVec[2] > 0.0) // (+,0,+)
                    Case0(0, 2, 1, linePoint, lineVec, boxExtent, result);
                else // (+,0,0)
                    Case00(0, 1, 2, linePoint, lineVec, boxExtent, result);
            }
        }
        else
        {
            if (lineVec[1] > 0.0)
            {
                if (lineVec[2] > 0.0) // (0,+,+)
                    Case0(1, 2, 0, linePoint, lineVec, boxExtent, result);
                else // (0,+,0)
                    Case00(1, 0, 2, linePoint, lineVec, boxExtent, result);
            }
            else
            {
                if (lineVec[2] > 0.0) // (0,0,+)
                    Case00(2, 0, 1, linePoint, lineVec, boxExtent, result);
                else
                    Case000(linePoint, boxExtent, result);
            }
        }

        // Undo the reflections applied previously.
        for (int i = 0; i < 3; ++i)
        {
            if (reflect[i])
                linePoint[i] = -linePoint[i];
        }

        result->distance = std::sqrt(result->squaredDistance);
        result->closestPoint[0] = line.PointAt(result->lineParameter);
        result->closestPoint[1] = boxCenter + linePoint;
    }

    void Face(int i0, int i1, int i2,
              Point& linePoint, const Vector& lineVec,
              const Vector& PmE, const Vector& boxExtent,
              DCPQuery<Line, AlignedBox>::ComputeResult* result)
    {
        Vector PpE;
        double lenSqr, inv, tmp, param, t, delta;

        PpE[i1] = linePoint[i1] + boxExtent[i1];
        PpE[i2] = linePoint[i2] + boxExtent[i2];

        if (lineVec[i0] * PpE[i1] >= lineVec[i1] * PmE[i0])
        {
            if (lineVec[i0] * PpE[i2] >= lineVec[i2] * PmE[i0])
            {
                // v[i1] >= -e[i1], v[i2] >= -e[i2] (distance = 0)
                linePoint[i0] = boxExtent[i0];
                inv = (1.0) / lineVec[i0];
                linePoint[i1] -= lineVec[i1] * PmE[i0] * inv;
                linePoint[i2] -= lineVec[i2] * PmE[i0] * inv;
                result->lineParameter = -PmE[i0] * inv;
            }
            else
            {
                // v[i1] >= -e[i1], v[i2] < -e[i2]
                lenSqr = lineVec[i0] * lineVec[i0] + lineVec[i2] * lineVec[i2];
                tmp = lenSqr * PpE[i1] - lineVec[i1] * (lineVec[i0] * PmE[i0] +
                                                        lineVec[i2] * PpE[i2]);
                if (tmp <= (2.0) * lenSqr * boxExtent[i1])
                {
                    t = tmp / lenSqr;
                    lenSqr += lineVec[i1] * lineVec[i1];
                    tmp = PpE[i1] - t;
                    delta = lineVec[i0] * PmE[i0] + lineVec[i1] * tmp + lineVec[i2] * PpE[i2];
                    param = -delta / lenSqr;
                    result->squaredDistance += PmE[i0] * PmE[i0] + tmp * tmp +
                        PpE[i2] * PpE[i2] + delta * param;

                    result->lineParameter = param;
                    linePoint[i0] = boxExtent[i0];
                    linePoint[i1] = t - boxExtent[i1];
                    linePoint[i2] = -boxExtent[i2];
                }
                else
                {
                    lenSqr += lineVec[i1] * lineVec[i1];
                    delta = lineVec[i0] * PmE[i0] + lineVec[i1] * PmE[i1] + lineVec[i2] * PpE[i2];
                    param = -delta / lenSqr;
                    result->squaredDistance += PmE[i0] * PmE[i0] + PmE[i1] * PmE[i1]
                        + PpE[i2] * PpE[i2] + delta * param;

                    result->lineParameter = param;
                    linePoint[i0] = boxExtent[i0];
                    linePoint[i1] = boxExtent[i1];
                    linePoint[i2] = -boxExtent[i2];
                }
            }
        }
        else
        {
            if (lineVec[i0] * PpE[i2] >= lineVec[i2] * PmE[i0])
            {
                // v[i1] < -e[i1], v[i2] >= -e[i2]
                lenSqr = lineVec[i0] * lineVec[i0] + lineVec[i1] * lineVec[i1];
                tmp = lenSqr * PpE[i2] - lineVec[i2] * (lineVec[i0] * PmE[i0] +
                                                        lineVec[i1] * PpE[i1]);
                if (tmp <= (2.0) * lenSqr * boxExtent[i2])
                {
                    t = tmp / lenSqr;
                    lenSqr += lineVec[i2] * lineVec[i2];
                    tmp = PpE[i2] - t;
                    delta = lineVec[i0] * PmE[i0] + lineVec[i1] * PpE[i1] + lineVec[i2] * tmp;
                    param = -delta / lenSqr;
                    result->squaredDistance += PmE[i0] * PmE[i0] + PpE[i1] * PpE[i1] +
                        tmp * tmp + delta * param;

                    result->lineParameter = param;
                    linePoint[i0] = boxExtent[i0];
                    linePoint[i1] = -boxExtent[i1];
                    linePoint[i2] = t - boxExtent[i2];
                }
                else
                {
                    lenSqr += lineVec[i2] * lineVec[i2];
                    delta = lineVec[i0] * PmE[i0] + lineVec[i1] * PpE[i1] + lineVec[i2] * PmE[i2];
                    param = -delta / lenSqr;
                    result->squaredDistance += PmE[i0] * PmE[i0] + PpE[i1] * PpE[i1] +
                        PmE[i2] * PmE[i2] + delta * param;

                    result->lineParameter = param;
                    linePoint[i0] = boxExtent[i0];
                    linePoint[i1] = -boxExtent[i1];
                    linePoint[i2] = boxExtent[i2];
                }
            }
            else
            {
                // v[i1] < -e[i1], v[i2] < -e[i2]
                lenSqr = lineVec[i0] * lineVec[i0] + lineVec[i2] * lineVec[i2];
                tmp = lenSqr * PpE[i1] - lineVec[i1] * (lineVec[i0] * PmE[i0] +
                                                        lineVec[i2] * PpE[i2]);
                if (tmp >= 0.0)
                {
                    // v[i1]-edge is closest
                    if (tmp <= (2.0) * lenSqr * boxExtent[i1])
                    {
                        t = tmp / lenSqr;
                        lenSqr += lineVec[i1] * lineVec[i1];
                        tmp = PpE[i1] - t;
                        delta = lineVec[i0] * PmE[i0] + lineVec[i1] * tmp + lineVec[i2] * PpE[i2];
                        param = -delta / lenSqr;
                        result->squaredDistance += PmE[i0] * PmE[i0] + tmp * tmp +
                            PpE[i2] * PpE[i2] + delta * param;

                        result->lineParameter = param;
                        linePoint[i0] = boxExtent[i0];
                        linePoint[i1] = t - boxExtent[i1];
                        linePoint[i2] = -boxExtent[i2];
                    }
                    else
                    {
                        lenSqr += lineVec[i1] * lineVec[i1];
                        delta = lineVec[i0] * PmE[i0] + lineVec[i1] * PmE[i1]
                            + lineVec[i2] * PpE[i2];
                        param = -delta / lenSqr;
                        result->squaredDistance += PmE[i0] * PmE[i0] + PmE[i1] * PmE[i1]
                            + PpE[i2] * PpE[i2] + delta * param;

                        result->lineParameter = param;
                        linePoint[i0] = boxExtent[i0];
                        linePoint[i1] = boxExtent[i1];
                        linePoint[i2] = -boxExtent[i2];
                    }
                    return;
                }

                lenSqr = lineVec[i0] * lineVec[i0] + lineVec[i1] * lineVec[i1];
                tmp = lenSqr * PpE[i2] - lineVec[i2] * (lineVec[i0] * PmE[i0] +
                                                        lineVec[i1] * PpE[i1]);
                if (tmp >= 0.0)
                {
                    // v[i2]-edge is closest
                    if (tmp <= (2.0) * lenSqr * boxExtent[i2])
                    {
                        t = tmp / lenSqr;
                        lenSqr += lineVec[i2] * lineVec[i2];
                        tmp = PpE[i2] - t;
                        delta = lineVec[i0] * PmE[i0] + lineVec[i1] * PpE[i1] + lineVec[i2] * tmp;
                        param = -delta / lenSqr;
                        result->squaredDistance += PmE[i0] * PmE[i0] + PpE[i1] * PpE[i1] +
                            tmp * tmp + delta * param;

                        result->lineParameter = param;
                        linePoint[i0] = boxExtent[i0];
                        linePoint[i1] = -boxExtent[i1];
                        linePoint[i2] = t - boxExtent[i2];
                    }
                    else
                    {
                        lenSqr += lineVec[i2] * lineVec[i2];
                        delta = lineVec[i0] * PmE[i0] + lineVec[i1] * PpE[i1]
                            + lineVec[i2] * PmE[i2];
                        param = -delta / lenSqr;
                        result->squaredDistance += PmE[i0] * PmE[i0] + PpE[i1] * PpE[i1]
                            + PmE[i2] * PmE[i2] + delta * param;

                        result->lineParameter = param;
                        linePoint[i0] = boxExtent[i0];
                        linePoint[i1] = -boxExtent[i1];
                        linePoint[i2] = boxExtent[i2];
                    }
                    return;
                }

                // (v[i1],v[i2])-corner is closest
                lenSqr += lineVec[i2] * lineVec[i2];
                delta = lineVec[i0] * PmE[i0] + lineVec[i1] * PpE[i1] + lineVec[i2] * PpE[i2];
                param = -delta / lenSqr;
                result->squaredDistance += PmE[i0] * PmE[i0] + PpE[i1] * PpE[i1]
                    + PpE[i2] * PpE[i2] + delta * param;

                result->lineParameter = param;
                linePoint[i0] = boxExtent[i0];
                linePoint[i1] = -boxExtent[i1];
                linePoint[i2] = -boxExtent[i2];
            }
        }
    }

    void CaseNoZero(Point& linePoint, const Vector& lineVec,
                    const Vector& boxExtent,
                    DCPQuery<Line, AlignedBox>::ComputeResult* result)
    {
        Vector PmE = linePoint - boxExtent;
        double prodDxPy = lineVec[0] * PmE[1];
        double prodDyPx = lineVec[1] * PmE[0];
        double prodDzPx, prodDxPz, prodDzPy, prodDyPz;

        if (prodDyPx >= prodDxPy)
        {
            prodDzPx = lineVec[2] * PmE[0];
            prodDxPz = lineVec[0] * PmE[2];
            if (prodDzPx >= prodDxPz)
            {
                // line intersects x = e0
                Face(0, 1, 2, linePoint, lineVec, PmE, boxExtent, result);
            }
            else
            {
                // line intersects z = e2
                Face(2, 0, 1, linePoint, lineVec, PmE, boxExtent, result);
            }
        }
        else
        {
            prodDzPy = lineVec[2] * PmE[1];
            prodDyPz = lineVec[1] * PmE[2];
            if (prodDzPy >= prodDyPz)
            {
                // line intersects y = e1
                Face(1, 2, 0, linePoint, lineVec, PmE, boxExtent, result);
            }
            else
            {
                // line intersects z = e2
                Face(2, 0, 1, linePoint, lineVec, PmE, boxExtent, result);
            }
        }
    }

    void Case0(int i0, int i1, int i2,
               Point& linePoint, const Vector& lineVec,
               const Vector& boxExtent,
               DCPQuery<Line, AlignedBox>::ComputeResult* result)
    {
        double PmE0 = linePoint[i0] - boxExtent[i0];
        double PmE1 = linePoint[i1] - boxExtent[i1];
        double prod0 = lineVec[i1] * PmE0;
        double prod1 = lineVec[i0] * PmE1;
        double delta, invLSqr, inv;

        if (prod0 >= prod1)
        {
            // line intersects P[i0] = e[i0]
            linePoint[i0] = boxExtent[i0];

            double PpE1 = linePoint[i1] + boxExtent[i1];
            delta = prod0 - lineVec[i0] * PpE1;
            if (delta >= 0.0)
            {
                invLSqr = (1.0) / (lineVec[i0] * lineVec[i0] + lineVec[i1] * lineVec[i1]);
                result->squaredDistance += delta * delta * invLSqr;
                linePoint[i1] = -boxExtent[i1];
                result->lineParameter = -(lineVec[i0] * PmE0 + lineVec[i1] * PpE1) * invLSqr;
            }
            else
            {
                inv = (1.0) / lineVec[i0];
                linePoint[i1] -= prod0 * inv;
                result->lineParameter = -PmE0 * inv;
            }
        }
        else
        {
            // line intersects P[i1] = e[i1]
            linePoint[i1] = boxExtent[i1];

            double PpE0 = linePoint[i0] + boxExtent[i0];
            delta = prod1 - lineVec[i1] * PpE0;
            if (delta >= 0.0)
            {
                invLSqr = (1.0) / (lineVec[i0] * lineVec[i0] + lineVec[i1] * lineVec[i1]);
                result->squaredDistance += delta * delta * invLSqr;
                linePoint[i0] = -boxExtent[i0];
                result->lineParameter = -(lineVec[i0] * PpE0 + lineVec[i1] * PmE1) * invLSqr;
            }
            else
            {
                inv = (1.0) / lineVec[i1];
                linePoint[i0] -= prod1 * inv;
                result->lineParameter = -PmE1 * inv;
            }
        }

        if (linePoint[i2] < -boxExtent[i2])
        {
            delta = linePoint[i2] + boxExtent[i2];
            result->squaredDistance += delta * delta;
            linePoint[i2] = -boxExtent[i2];
        }
        else if (linePoint[i2] > boxExtent[i2])
        {
            delta = linePoint[i2] - boxExtent[i2];
            result->squaredDistance += delta * delta;
            linePoint[i2] = boxExtent[i2];
        }
    }

    void Case00(int i0, int i1, int i2,
                Point& linePoint, const Vector& lineVec,
                const Vector& boxExtent,
                DCPQuery<Line, AlignedBox>::ComputeResult* result)
    {
        double delta;

        result->lineParameter = (boxExtent[i0] - linePoint[i0]) / lineVec[i0];

        linePoint[i0] = boxExtent[i0];

        if (linePoint[i1] < -boxExtent[i1])
        {
            delta = linePoint[i1] + boxExtent[i1];
            result->squaredDistance += delta * delta;
            linePoint[i1] = -boxExtent[i1];
        }
        else if (linePoint[i1] > boxExtent[i1])
        {
            delta = linePoint[i1] - boxExtent[i1];
            result->squaredDistance += delta * delta;
            linePoint[i1] = boxExtent[i1];
        }

        if (linePoint[i2] < -boxExtent[i2])
        {
            delta = linePoint[i2] + boxExtent[i2];
            result->squaredDistance += delta * delta;
            linePoint[i2] = -boxExtent[i2];
        }
        else if (linePoint[i2] > boxExtent[i2])
        {
            delta = linePoint[i2] - boxExtent[i2];
            result->squaredDistance += delta * delta;
            linePoint[i2] = boxExtent[i2];
        }
    }

    void Case000(Point& linePoint,
                 const Vector& boxExtent,
                 DCPQuery<Line, AlignedBox>::ComputeResult* result)
    {
        double delta;

        if (linePoint[0] < -boxExtent[0])
        {
            delta = linePoint[0] + boxExtent[0];
            result->squaredDistance += delta * delta;
            linePoint[0] = -boxExtent[0];
        }
        else if (linePoint[0] > boxExtent[0])
        {
            delta = linePoint[0] - boxExtent[0];
            result->squaredDistance += delta * delta;
            linePoint[0] = boxExtent[0];
        }

        if (linePoint[1] < -boxExtent[1])
        {
            delta = linePoint[1] + boxExtent[1];
            result->squaredDistance += delta * delta;
            linePoint[1] = -boxExtent[1];
        }
        else if (linePoint[1] > boxExtent[1])
        {
            delta = linePoint[1] - boxExtent[1];
            result->squaredDistance += delta * delta;
            linePoint[1] = boxExtent[1];
        }

        if (linePoint[2] < -boxExtent[2])
        {
            delta = linePoint[2] + boxExtent[2];
            result->squaredDistance += delta * delta;
            linePoint[2] = -boxExtent[2];
        }
        else if (linePoint[2] > boxExtent[2])
        {
            delta = linePoint[2] - boxExtent[2];
            result->squaredDistance += delta * delta;
            linePoint[2] = boxExtent[2];
        }
    }
}