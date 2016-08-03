#version 400

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7

layout (lines) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 viewProjection;
uniform sampler1D gradient;

in Vertex
{
    float angle;
    vec2 radiusRange;
    vec2 center;
    vec2 heightRange;
    vec3 color;
    bool hasSide;
} vertex[];

flat out vec3 g_color;
flat out vec3 g_normal;

vec3 segment[8];

void emit(in vec3 pos, in vec3 n)
{
    gl_Position = viewProjection * vec4(pos, 1.0);
    g_color = vertex[0].color;
    g_normal = n;
    
    EmitVertex();
}

vec3 circlePoint(in float angle, in float radius, in float height, in vec2 center)
{
    float x = sin(angle) * radius + center.x;
    float y = cos(angle) * radius + center.y;
    
    return vec3(x, height, y);
}

void emitPrimitive10(in int indices[10])
{
    vec3 prevPrev = segment[indices[0]];
    vec3 prev = segment[indices[1]];
    
    emit(prevPrev, vec3(0.0));
    emit(prev, vec3(0.0));
    
    for (int i=2; i<10; ++i)
    {
        vec3 current = segment[indices[i]];
        vec3 n = cross(prev-prevPrev, current-prev) * (i%2==0?1.0:-1.0);
        emit(current, n);
        
        prevPrev = prev;
        prev = current;
    }
    
    EndPrimitive();
}

void emitPrimitive6(in int indices[6])
{
    vec3 prevPrev = segment[indices[0]];
    vec3 prev = segment[indices[1]];
    
    emit(prevPrev, vec3(0.0));
    emit(prev, vec3(0.0));
    
    for (int i=2; i<6; ++i)
    {
        vec3 current = segment[indices[i]];
        vec3 n = cross(prev-prevPrev, current-prev) * (i%2==0?1.0:-1.0);
        emit(current, n);
        
        prevPrev = prev;
        prev = current;
    }
    
    EndPrimitive();
}

void emitPrimitive4(in int indices[4])
{
    vec3 prevPrev = segment[indices[0]];
    vec3 prev = segment[indices[1]];
    
    emit(prevPrev, vec3(0.0));
    emit(prev, vec3(0.0));
    
    for (int i=2; i<4; ++i)
    {
        vec3 current = segment[indices[i]];
        vec3 n = cross(prev-prevPrev, current-prev) * (i%2==0?1.0:-1.0);
        emit(current, n);
        
        prevPrev = prev;
        prev = current;
    }
    
    EndPrimitive();
}

void main()
{
    float startAngle = vertex[0].angle;
    float endAngle = vertex[1].angle;
    float innerRadius = vertex[0].radiusRange[0];
    float outerRadius = vertex[0].radiusRange[1];
    float startBottom = vertex[0].heightRange[0];
    float endBottom = vertex[1].heightRange[0];
    float startTop = vertex[0].heightRange[1];
    float endTop = vertex[1].heightRange[1];
    vec2 center = vertex[0].center;
    
    segment[A] = circlePoint(startAngle, innerRadius, startBottom, center);
    segment[B] = circlePoint(endAngle, innerRadius, endBottom, center);
    segment[C] = circlePoint(endAngle, outerRadius, endBottom, center);
    segment[D] = circlePoint(startAngle, outerRadius, startBottom, center);
    segment[E] = circlePoint(startAngle, innerRadius, startTop, center);
    segment[F] = circlePoint(endAngle, innerRadius, endTop, center);
    segment[G] = circlePoint(endAngle, outerRadius, endTop, center);
    segment[H] = circlePoint(startAngle, outerRadius, startTop, center);
        
    if (startTop - startBottom > 0.0 || endTop - endBottom > 0.0)
    {
        if (vertex[0].hasSide)
        {
            emitPrimitive4(int[4](A, D, E, H));
        }
        
        emitPrimitive10(int[10](B, A, F, E, G, H, C, D, B, A));
        
        if (vertex[1].hasSide)
        {
            emitPrimitive4(int[4](C, B, G, F));
        }
    }
    else
    {
        emitPrimitive6(int[6](E, F, H, G, E, F));
    }
}
