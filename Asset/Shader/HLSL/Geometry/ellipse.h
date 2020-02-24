#ifndef _ELLIPSE_H_
#define _ELLIPSE_H_

#include "primitive.h"

struct EllipseAttr_Dash
{
    float2 center : POSITION;   // screen space center
    float2 radius : RADIUS;     // screen space radius

    float2 range : RANGE;       // start angle end angle
    float rotate : ROTATE;      // rotation

    uint color : COLOR0;        // color
    float drawZ : DRAWZ;        // draw order z
};
#define ATTR_SIZE (3)           // count of attribute size (of float3)

struct VertexAttr_Ellipse
{
    noperspective float4 position : SV_Position;    // transformed  vertex position

    nointerpolation  float2 radius : RADIUS;        // x long axis, y short axis
    nointerpolation  float2 range : RANGE;          // x start angle, y end angle
    nointerpolation  float2 center : POSITION;      // screen space center
    nointerpolation  float rotate : ROTATE;         // rotation

    linear float2 uv : UV;                          // uv is used to compute gradient
    nointerpolation  uint  color : COLOR0;          // ellipse color
    nointerpolation  float weight : WEIGHT;         // ellipse line weight
};

// load ellipse attributes
void assign_attr(float3 attr_array[ATTR_SIZE], out EllipseAttr_Dash attr)
{
    attr.center.x = attr_array[0].x;
    attr.center.y = attr_array[0].y;
    attr.radius.x = attr_array[0].z;
    attr.radius.y = attr_array[1].x;

    attr.range    = attr_array[1].yz;
    attr.rotate   = attr_array[2].x;
    attr.color    = asuint(attr_array[2].y);

    attr.drawZ    = attr_array[2].z;
}

// load ellipse input info
void load_ellipse_info(Texture2D<float3> tex, uint offset, out EllipseAttr_Dash attr)
{
    float3 attr_array[ATTR_SIZE];

    [unroll]
    for (uint i = 0; i <ATTR_SIZE; ++i)
    {
        int2 tex_offset = get_ptex_offset(offset*ATTR_SIZE + i);
        attr_array[i] = tex.Load(int3(tex_offset, 0));
    }

    assign_attr(attr_array, attr);
}

float2 get_ellipse_model_pos(uint vid, float weight_expand, float2 mid_pos, float mid_dist, 
    float2 mid_vec, float2 mid_tan,
    float angle_delta)
{
    float cos_mn = abs(dot(mid_tan, mid_vec));
    float sin_mn = abs(sqrt(1 - cos_mn*cos_mn));

    float adjust_weight_expand = weight_expand / sin_mn;

    float2 model_pos;

    if ((vid & 0x1) == 0)
    {
        adjust_weight_expand = weight_expand;

        if (mid_dist <= adjust_weight_expand)
            model_pos = float2(0.0f, 0.0f);
        else
            model_pos = mid_pos - adjust_weight_expand*mid_vec;
    }
    else
    {
        float half_angle_delta = angle_delta / 2.0f;

        float2 sin_cos_half;
        sincos(half_angle_delta, sin_cos_half.x, sin_cos_half.y);

        float t_dist = (mid_dist + adjust_weight_expand) / sin_cos_half.y - mid_dist;

        if (t_dist < adjust_weight_expand)
            t_dist = adjust_weight_expand;

        model_pos = mid_pos + t_dist*mid_vec;
    }

    return model_pos;
}

float2 get_ellipse_degrade_pos(uint vid,  float2 center, inout float2 range, inout float weight, out float2 uv)
{
    float2 pos_s = int2(center);
    if (vid > 3)
        vid = 3;

    pos_s.x += (-0.5f + (float)(vid & 0x1));
    pos_s.y += (-0.5f + (float)(vid & 0x2));

    uv.x = (-0.5f + (float)(vid & 0x1));
    uv.y = (-0.5f + (float)(vid & 0x2));

    range = float2(0.0f, 2 * PI);
    weight = 1.0f;

    return screen_to_ndc_pos(pos_s);
}

float2 get_ndc_pos(float2 center, float sin_rot, float cos_rot, float2 model_pos)
{
    // rotate model pos
    float2 rot_pos;

    rot_pos.x = model_pos.x*cos_rot - model_pos.y*sin_rot;
    rot_pos.y = model_pos.x*sin_rot + model_pos.y*cos_rot;

    // get ndc pos
    float2 scr_pos = center + rot_pos;
    float2 ndc_pos = scr_pos * gInvScreenSize * 2.0f - 1.0f;

    return ndc_pos;
}

// compute envelope shape with 30 triangles
static const uint ENVELOPE_ANGLE_COUNT = 15;

// generate ellipse envelope shapes
float2 get_vertex_pos_envelope_30(uint vid, float long_radius, float short_radius,
    float2 center, float sin_rot, float cos_rot,
    inout float2 range, inout float weight, out float2 uv)
{
    if (max(long_radius, short_radius) < 0.5f)
        return get_ellipse_degrade_pos(vid,  center, range, weight, uv);

    float weight_expand = weight*4.0f;

    float2 expand_range = range + float2(-PI / 180.0f, PI / 180.0f);
    if (expand_range.x < 0.0f) expand_range.x = 0.0f;

    // get arc range index:
    float angle_delta = (2 * PI) / (float)ENVELOPE_ANGLE_COUNT;
    float2 range_index = expand_range / angle_delta;

    int s_index = (int)range_index.x;
    int e_index = (int)range_index.y + 1;

    if (e_index - s_index >= (int)ENVELOPE_ANGLE_COUNT)
        e_index = s_index + ENVELOPE_ANGLE_COUNT;

    // get angle_index
    int angle_index = (vid >> 1) + s_index;

    if (angle_index > e_index) // if not current arc, move to end angle to 
        angle_index = e_index; // generate degeneration shape

    while (angle_index > (int)ENVELOPE_ANGLE_COUNT) // change to 0..2PI
        angle_index -= ENVELOPE_ANGLE_COUNT;

    // get angle
    float cur_angle = angle_index * angle_delta;

    // get mid point parameters
    float2 sin_cos_angle;
    sincos(cur_angle, sin_cos_angle.x, sin_cos_angle.y);

    float2 mid_pos = float2(long_radius*sin_cos_angle.y, short_radius*sin_cos_angle.x);
    float  mid_dist = sqrt(mid_pos.x * mid_pos.x + mid_pos.y*mid_pos.y);
    float2 mid_vec = normalize(mid_pos);

    float2 mid_tan = float2(-long_radius*sin_cos_angle.x, short_radius*sin_cos_angle.y);
    mid_tan = normalize(mid_tan);

    // get model space position
    float2 model_pos = get_ellipse_model_pos(vid, weight_expand, mid_pos, mid_dist, mid_vec, mid_tan,
        angle_delta);

    uv = model_pos;

    // rotate model pos
    return get_ndc_pos(center, sin_rot, cos_rot, model_pos);
}

VertexAttr_Ellipse output_vertex_attr_ellipse(uint vertexId, EllipseAttr_Dash ellipse_attr)
{
    // initialize
    VertexAttr_Ellipse output = (VertexAttr_Ellipse)(0);

    // update geometry info
    float2 center = float2(ellipse_attr.center.x, ellipse_attr.center.y);
    output.range = ellipse_attr.range;

    float2 adjusted_radius = ellipse_attr.radius;

    float sin_rot, cos_rot;
    sincos(ellipse_attr.rotate, sin_rot, cos_rot);

    if (ellipse_attr.radius.x == 0) adjusted_radius.x = 0.5f;
    if (ellipse_attr.radius.y == 0) adjusted_radius.y = 0.5f;

    output.weight = 1.0;
    output.position.xy = get_vertex_pos_envelope_30(vertexId, adjusted_radius.x, adjusted_radius.y,
        center, sin_rot, cos_rot, output.range, output.weight, output.uv);

    // get the radius
    output.radius = adjusted_radius;

    output.center = center;
    output.rotate = ellipse_attr.rotate;

    // update other properties
    output.position.z = ellipse_attr.drawZ;
    output.position.w = 1.0f;

    output.color = ellipse_attr.color; // move color assignment to last will fix an Intel compiler issue.
                                       // since the color assignment will affect position result on Intel cards.

    return output;
}

// get angle for a point on ellipse 
float get_ellipse_angle(float2 radius, float2 uv)
{
    float x_len = radius.x;
    float y_len = radius.y;

    float cur_angle = atan2(uv.y / y_len, uv.x / x_len);

    if (cur_angle < 0)
        cur_angle = cur_angle + 2 * PI;

    return cur_angle;
}

// check if angle is in arc start/end angle
bool angle_is_in_range(float cur_angle, float2 range)
{
    float lower = min(range.x, range.y);
    float upper = max(range.x, range.y);
    float delta = upper - lower + EPS;

    float dist = cur_angle - lower;
    float dist2 = dist + 2 * PI;
    bool result = ((dist > -EPS) && (dist <= delta))
        || ((dist2 > -EPS) && (dist2 <= delta));

    return result;
}

// check current pixel is in arc start/end angle
bool valid_range(float2 radius, float2 uv, float2 range)
{
    float cur_angle = get_ellipse_angle(radius, uv);

    return angle_is_in_range(cur_angle, range);
}

#endif