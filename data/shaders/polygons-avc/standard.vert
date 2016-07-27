#version 430

struct PolygonAttributes
{
    vec2 center;
    
    float height;
    float z;
    
    int gradientIndex;
    float colorValue;
    float alpha;
    int id;
};

layout (std430, binding = 0) buffer Polygons
{
    PolygonAttributes polygonAttributes[];
};

in vec2 in_start;
in int in_startPolygonIndex;
in vec2 in_end;
in int in_endPolygonIndex;

out int v_matchingPolygon;
out vec2 v_start;
out vec2 v_end;
out float v_bottom;
out float v_top;
out vec3 v_color;

void main()
{
    PolygonAttributes attributes = polygonAttributes[in_startPolygonIndex];
    
    v_matchingPolygon = in_startPolygonIndex == in_endPolygonIndex ? 1 : 0;

    v_bottom = attributes.z;
    v_top = attributes.z + attributes.height;
    
    v_start = in_start;
    v_end = in_end;
    
    v_id = attributes.id;
    v_color = attributes.colorValue * mix(vec3(1, 0, 0), vec3(0, 1, 0), float(bool(attributes.gradientIndex)));
    
    gl_Position = vec4(attributes.center, 0.0, 1.0);
}
