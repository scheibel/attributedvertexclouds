#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

const vec3 UP = vec3(0.0, 1.0, 0.0);

uniform mat4 viewProjection;

uniform sampler1D centerAndHeights;
uniform sampler1D colorAndGradients;

in vec2 v_start[];
in int v_startIndex[];
in vec2 v_end[];
in int v_endIndex[];

flat out vec3 g_color;
flat out vec3 g_normal;

void emit(in vec3 pos, in vec3 n, in vec3 color)
{
    gl_Position = viewProjection * vec4(pos, 1.0);

    g_color = color;
    g_normal = n;
    
    EmitVertex();
}

void main()
{
    // Handle attributes for mixed prismas due to overlapping VAO configuration
    if (v_startIndex[0] != v_endIndex[0])
    {
        return;
    }
    
    int prismaIndex = v_startIndex[0];
    
    vec4 centerAndHeight = texelFetch(centerAndHeights, prismaIndex, 0);
    vec2 colorAndGradient = texelFetch(colorAndGradients, prismaIndex, 0).rg;
    
    vec3 color = colorAndGradient.r * mix(vec3(1, 0, 0), vec3(0, 1, 0), colorAndGradient.g);
    
    vec3 center = vec3(centerAndHeight.r, centerAndHeight.a, centerAndHeight.g);
    
    vec3 sBottom = vec3(v_start[0].x, centerAndHeight.b, v_start[0].y);
    vec3 eBottom = vec3(v_end[0].x, centerAndHeight.b, v_end[0].y);
    vec3 sTop = vec3(v_start[0].x, centerAndHeight.a, v_start[0].y);
    vec3 eTop = vec3(v_end[0].x, centerAndHeight.a, v_end[0].y);
        
    vec3 normal = cross(sBottom - eBottom, UP);
    
    emit(eBottom, normal, color);
    emit(sBottom, normal, color);
    emit(eTop, normal, color);
    emit(sTop, normal, color);
    emit(center, UP, color);
    
    EndPrimitive();
}
