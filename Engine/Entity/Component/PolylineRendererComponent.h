#pragma once

#include "Component.h"
#include "IRenderer.h"

#include <vector>

namespace Engine
{
    class PolylineRendererComponent : public ComponentBase<PolylineRendererComponent>
    {
    public:
        PolylineRendererComponent() : ComponentBase<PolylineRendererComponent>() {}
        virtual ~PolylineRendererComponent() = default;

        struct LineSegmentGeom
        {
            float2 startPoint;
            float2 endPoint;
            uint32_t color;
            float drawZ;
        };

        std::vector<LineSegmentGeom> GetGeometry() const;
        void SetGeometry(const LineSegmentGeom& geom);

    private:
        std::vector<LineSegmentGeom> m_geom;
    };
}