#version 330

flat in vec3 g_normal;
flat in int g_type;
in vec2 g_texCoord;

uniform sampler2DArray terrain;

uniform vec3 lightDirs[6] = vec3[](
    vec3( 0,  1,  0),
    vec3( 1,  0,  0),
    vec3(-1,  0,  0),
    vec3( 0,  0,  1),
    vec3( 0,  0, -1),
    vec3( 0, -1,  0)
);

uniform float lightStrengths[6] = float[](
    1.0,
    0.95,
    0.9,
    0.85,
    0.8,
    0.75
);

out vec3 out_color;

void main()
{
    if (g_type <= 0)
    {
        discard;
        return;
    }
    
    vec3 terrainColor = texture(terrain, vec3(g_texCoord, g_type-1)).rgb;
    vec3 col = vec3(0.0);
    vec3 N = normalize(g_normal);

    for (int i = 0; i < 6; ++i)
    {
        vec3 L = lightDirs[i];
        float lambertTerm = dot(N,L);

        col += max(lambertTerm, 0.0) * terrainColor * lightStrengths[i];
    }
    
    out_color = col;
}
