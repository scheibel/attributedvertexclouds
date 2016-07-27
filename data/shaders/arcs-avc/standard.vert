#version 150

in vec2  in_center;
in vec2  in_heightRange;
in vec2  in_angleRange;
in vec2  in_radiusRange;
in float in_colorValue;
in int   in_gradientIndex;

out Segment
{
    vec2 angleRange;
    vec2 radiusRange;
    vec2 center;
    vec2 heightRange;
    vec3 color;
} segment;

void main()
{
    segment.angleRange = in_angleRange;
    segment.radiusRange = in_radiusRange;
    segment.heightRange = in_heightRange;
    segment.center = in_center;

    segment.color = in_colorValue * mix(vec3(1, 0, 0), vec3(0, 1, 0), float(bool(in_gradientIndex)));
}
