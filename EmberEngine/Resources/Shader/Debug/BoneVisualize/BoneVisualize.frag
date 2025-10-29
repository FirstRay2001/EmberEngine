#version 330 core

in vec3 BoneColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(BoneColor, 1.0);
}