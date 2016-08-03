#version 330

uniform mat4 viewProjection;

in vec3 in_vertex;
in vec3 in_normal;
in int  in_type;

flat out vec3 g_normal;
flat out int  g_type;
     out vec2 g_texCoord;

void main()
{
    gl_Position = viewProjection * vec4(in_vertex, 1.0);
    
    g_normal = in_normal;
    g_type = in_type;
    g_texCoord = vec2(0.0, 0.0);
}
