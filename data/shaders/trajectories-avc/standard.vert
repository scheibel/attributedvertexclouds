#version 150

in vec3  in_position;
in int   in_type;
in vec3  in_incoming;
in vec3  in_outgoing;
in float in_colorValue;
in float in_sizeValue;

out Segment
{
    vec3  position;
    int   type;
    vec3  incoming;
    vec3  outgoing;
    float colorValue;
    float sizeValue;
} segment;

void main()
{
    gl_Position = vec4(in_position, 1.0);
    
    segment.position = in_position;
    segment.type = in_type;
    segment.incoming = in_incoming;
    segment.outgoing = in_outgoing;
    segment.colorValue = in_colorValue;
    segment.sizeValue = in_sizeValue;
}
