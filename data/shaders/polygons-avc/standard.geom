#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

uniform mat4 viewProjection;

in int v_matchingPolygon[];
in vec2 v_start[];
in vec2 v_end[];
in float v_bottom[];
in float v_top[];
in vec3 v_color[];

flat out vec3 g_color;
flat out vec3 g_normal;

void emit(in vec3 pos, in vec3 n, in vec3 tex)
{
    gl_Position = viewProjection * vec4(pos, 1.0);

    g_color = v_color[0];
    g_normal = n;
    
    EmitVertex();
}

void main()
{
    if (!bool(v_matchingPolygon[0]))
    {
        return;
    }
    
    vec3 center = vec3(gl_in[0].gl_Position.x, v_top[0], gl_in[0].gl_Position.y);
    
    vec3 sBottom = vec3(v_start[0].x, v_bottom[0], v_start[0].y);
    vec3 eBottom = vec3(v_end[0].x, v_bottom[0], v_end[0].y);
    vec3 sTop = vec3(v_start[0].x, v_top[0], v_start[0].y);
    vec3 eTop = vec3(v_end[0].x, v_top[0], v_end[0].y);
        
    vec3 normal = cross(sBottom - eBottom, UP);
    
    emit(eBottom, normal, vec3(0.0));
    emit(sBottom, normal, vec3(0.0));
    emit(eTop, normal, vec3(0.0));
    emit(sTop, normal, vec3(0.0));
    emit(center, vec3(0.0, 1.0, 0.0), vec3(0.0));
    
    EndPrimitive();
}
