#version 150

flat in vec3 g_color;
flat in vec3 g_normal;

out vec3 out_color;

vec3 lightDirs[5] = vec3[](
    vec3( 0,  1,  0),
    vec3( 1,  0,  0),
    vec3(-1,  0,  0),
    vec3( 0,  0,  1),
    vec3( 0,  0, -1)
);

float lightStrengths[5] = float[](
    1.0,
    0.9,
    0.9,
    0.95,
    0.95
);

vec3 shading(in vec3 color, in vec3 normal)
{    
    vec3 col = vec3(0.0);
    vec3 N = normalize(normal);

    for (int i = 0; i < 5; ++i)
    {
        vec3 L = lightDirs[i];
        float lambertTerm = dot(N,L);

        col += max(lambertTerm, 0.0) * color * lightStrengths[i];
    }
    
    return col;
}

void main()
{
    out_color = shading(g_color, g_normal);
}
