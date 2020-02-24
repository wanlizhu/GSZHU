#pragma once

#include "Component.h"
#include "IRenderer.h"

#include <vector>

namespace Engine
{
    class EllipseArcRendererComponent : public ComponentBase<EllipseArcRendererComponent>
    {
    public:
        EllipseArcRendererComponent() : ComponentBase<EllipseArcRendererComponent>() {}
        virtual ~EllipseArcRendererComponent() = default;

        struct EllipseArcGeom
        {
            float2 center;
            float2 radius;
            float2 range;
            float  rotate;
            
            uint32_t color;
            float drawZ;
        };

        std::vector<EllipseArcGeom> GetGeometry() const;
        void SetGeometry(const EllipseArcGeom& geom);

    private:
        std::vector<EllipseArcGeom> m_geom;
    };
}