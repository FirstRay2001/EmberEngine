#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 position;
    vec3 direction;
} gs_in[];

out vec3 BoneColor;

void main()
{
}