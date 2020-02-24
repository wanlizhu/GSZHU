#include "EllipseArcRendererComponent.h"

using namespace Engine;

std::vector<EllipseArcRendererComponent::EllipseArcGeom> EllipseArcRendererComponent::GetGeometry() const
{
    return m_geom;
}

void EllipseArcRendererComponent::SetGeometry(const EllipseArcRendererComponent::EllipseArcGeom& geom)
{
    m_geom.push_back(geom);
}
