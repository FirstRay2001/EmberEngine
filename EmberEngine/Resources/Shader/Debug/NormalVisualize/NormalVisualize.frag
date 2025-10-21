#version 330 core

in vec3 NormalColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(NormalColor, 1.0);
}