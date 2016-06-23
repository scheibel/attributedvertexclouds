#version 330
#extension GL_ARB_shading_language_include : require

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

uniform mat4 viewProjection;

in vec2 v_extent[];
in vec3 v_color[];
in float v_height[];

flat out vec3 g_color;
flat out vec3 g_normal;

uniform float openCuboidScaleFix = 1.0;


const vec3 NEGATIVE_X = vec3(-1.0, 0.0, 0.0);
const vec3 NEGATIVE_Y = vec3(0.0, -1.0, 0.0);
const vec3 NEGATIVE_Z = vec3(0.0, 0.0, -1.0);
const vec3 POSITIVE_X = vec3(1.0, 0.0, 0.0);
const vec3 POSITIVE_Y = vec3(0.0, 1.0, 0.0);
const vec3 POSITIVE_Z = vec3(0.0, 0.0, 1.0);

// To be defined by using GLSL code
void openCuboidEmit(in vec4 position, in vec3 normal);

void generateOpenCuboid(in vec3 center, in vec3 scale)
{
    if (scale.x <= 0.0 || scale.z <= 0.0)
    {
        return;
    }
    
    vec3 llf = center - (vec3(scale.x, scale.y * openCuboidScaleFix, scale.z) / vec3(2.0));
    vec3 urb = center + (vec3(scale.x, scale.y * openCuboidScaleFix, scale.z) / vec3(2.0));

    vec4 vertices[8];
    vertices[0] = vec4(llf.x, urb.y, llf.z, 1.0); // A = H
    vertices[1] = vec4(llf.x, urb.y, urb.z, 1.0); // B = F
    vertices[2] = vec4(urb.x, urb.y, llf.z, 1.0); // C = J
    vertices[3] = vec4(urb.x, urb.y, urb.z, 1.0); // D
    vertices[4] = vec4(urb.x, llf.y, urb.z, 1.0); // E = L
    vertices[5] = vec4(llf.x, llf.y, urb.z, 1.0); // G
    vertices[6] = vec4(llf.x, llf.y, llf.z, 1.0); // I
    vertices[7] = vec4(urb.x, llf.y, llf.z, 1.0); // K
    
    openCuboidEmit(vertices[0], POSITIVE_Y); // A
    EmitVertex();
    openCuboidEmit(vertices[1], POSITIVE_Y); // B
    EmitVertex();
    openCuboidEmit(vertices[2], POSITIVE_Y); // C
    EmitVertex();
    openCuboidEmit(vertices[3], POSITIVE_Y); // D
    EmitVertex();
    
    if (scale.y > 0.0)
    {
        openCuboidEmit(vertices[4], POSITIVE_X); // E
        EmitVertex();

        openCuboidEmit(vertices[1], POSITIVE_Z); // F
        EmitVertex();
        openCuboidEmit(vertices[5], POSITIVE_Z); // G
        EmitVertex();

        openCuboidEmit(vertices[0], NEGATIVE_X); // H
        EmitVertex();
        openCuboidEmit(vertices[6], NEGATIVE_X); // I
        EmitVertex();

        openCuboidEmit(vertices[2], NEGATIVE_Z); // J
        EmitVertex();
        openCuboidEmit(vertices[7], NEGATIVE_Z); // K
        EmitVertex();

        openCuboidEmit(vertices[4], POSITIVE_X); // L
        EmitVertex();
    }
    
    EndPrimitive();
}


// is called up to #OPEN_CUBOID_VERTICES# times,
// each one with the world position of the current vertex and it's normal (regarding the provoking vertex)
void openCuboidEmit(in vec4 position, in vec3 normal)
{
    gl_Position = viewProjection * position;
    g_normal = normal;
    g_color = v_color[0];
}

void main()
{
    vec3 center = gl_in[0].gl_Position.xyz;
    center.y += v_height[0] / 2.0;
    vec3 scale = vec3(v_extent[0].x, v_height[0], v_extent[0].y);
    
    generateOpenCuboid(center, scale);
}
