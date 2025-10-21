#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 normal;
} gs_in[];

out vec3 NormalColor;

const float MAGNITUDE = 0.4;

uniform mat4 projection;


void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    NormalColor = vec3(0.1, 0.1, 0.1);
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    NormalColor = vec3(0.8, 0.8, 0.2);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}