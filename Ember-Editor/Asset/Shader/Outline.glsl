#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform float u_OutlineThickness;

void main()
{
    // 膨胀顶点
    float scaleFactor = 1.0 + u_OutlineThickness;
    vec3 scaledPosition = a_Position * scaleFactor;
    
    gl_Position = u_ViewProjection * u_Transform * vec4(scaledPosition.xyz, 1.0);
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