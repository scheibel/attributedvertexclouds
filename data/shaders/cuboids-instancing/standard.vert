#version 330

uniform mat4 viewProjection;

in vec3  in_vertex;
in vec3  in_normal;
in vec3  in_position;
in vec3  in_scale;
in float in_colorValue;
in int   in_gradientIndex;

flat out vec3 g_color;
flat out vec3 g_normal;

void main()
{
    gl_Position = viewProjection * vec4(in_vertex * in_scale + in_position, 1.0);
    
    g_color = in_colorValue * mix(vec3(1, 0, 0), vec3(0, 1, 0), float(bool(in_gradientIndex)));
    g_normal = in_normal;
}
