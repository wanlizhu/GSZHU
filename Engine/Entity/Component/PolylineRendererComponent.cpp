#include "PolylineRendererComponent.h"

using namespace Engine;

std::vector<PolylineRendererComponent::LineSegmentGeom> PolylineRendererComponent::GetGeometry() const
{
    return m_geom;
}

void PolylineRendererComponent::SetGeometry(const PolylineRendererComponent::LineSegmentGeom& geom)
{
    m_geom.push_back(geom);
}