#include "basic.h"

cbuffer TransformCB : register(b0)
{
    row_major float4x4 gWorldMatrix : WORLD;
    row_major float4x4 gViewMatrix : VIEW;
    row_major float4x4 gProjectionView : PROJECTION;
};

Basic_VertexAttr Basic_VS(Basic_Input input)
{
    Basic_VertexAttr output = (Basic_VertexAttr)0;

    output.position.xyz = input.Position.xyz;
    output.position.w = 1.0f;

    output.position = mul(output.position, gWorldMatrix);
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionView);

    return output;
}