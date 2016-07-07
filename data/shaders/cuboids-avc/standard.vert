#version 330

in vec2  in_center;
in vec2  in_extent;
in vec2  in_heightRange;
in float in_colorValue;
in int   in_gradientIndex;

out vec2 v_extent;
out vec3 v_color;
out float v_height;

void main()
{
    gl_Position = vec4(in_center.x, in_heightRange.x, in_center.y, 1.0);
    
    v_extent = in_extent;
    v_color = in_colorValue * mix(vec3(1, 0, 0), vec3(0, 1, 0), float(bool(in_gradientIndex)));
    v_height = in_heightRange.y;
}
