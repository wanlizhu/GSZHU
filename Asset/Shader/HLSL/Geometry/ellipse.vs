#include "ellipse.h"

Texture2D<float3> gEllipseArcTex : register(t0);

VertexAttr_Ellipse Ellipse_Dash_VS(NullVertex_Input input)
{
     // load ellipse information
    EllipseAttr_Dash ellipse_attr;
    load_ellipse_info(gEllipseArcTex, input.InstanceID, ellipse_attr);

    return output_vertex_attr_ellipse(input.VertexID, ellipse_attr);
}