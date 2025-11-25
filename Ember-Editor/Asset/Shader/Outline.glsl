#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform float u_OutlineThickness;

void main()
{
    // 沿着法线方向膨胀顶点位置
    vec3 outlinePosition = a_Position + a_Normal * u_OutlineThickness;
    gl_Position = u_ViewProjection * u_Transform * vec4(outlinePosition, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

uniform vec4 u_OutlineColor;

void main()
{
    FragColor = u_OutlineColor;
    EntityID = -1;
}