#version 400

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 viewProjection;

in Vertex
{
    vec3 center;
    vec3 normal;
    vec3 bitangent;
    vec3 color;
    int type;
} vertex[];

flat out vec3 g_color;
flat out vec3 g_normal;

void main()
{
    if (vertex[0].type == 2)
    {
        return;
    }
    
    g_color = vertex[0].color;
    g_normal = vertex[0].normal;
    gl_Position = viewProjection * gl_in[1].gl_Position;
    EmitVertex();
    
    g_color = vertex[0].color;
    g_normal = vertex[0].normal;
    gl_Position = viewProjection * (gl_in[1].gl_Position + vec4(vertex[0].bitangent, 1.0));
    EmitVertex();
    
    g_color = vertex[0].color;
    g_normal = vertex[0].normal;
    gl_Position = viewProjection * gl_in[0].gl_Position;
    EmitVertex();
    
    //if (vertex[0].type == 2) // sphere / line
    {
        g_color = vertex[0].color;
        g_normal = vertex[0].normal;
        gl_Position = viewProjection * (gl_in[0].gl_Position + vec4(vertex[0].bitangent, 1.0));
        EmitVertex();
    }
    
    EndPrimitive();
}
