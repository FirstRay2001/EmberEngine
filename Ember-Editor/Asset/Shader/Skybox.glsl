// 顶点着色器
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 Position;

void main()
{
	gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
    Position = a_Position;
}


// 片段着色器
#type fragment
#version 330 core

in vec3 Position;

uniform samplerCube u_Cubemap;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

void main()
{
    FragColor = texture(u_Cubemap, Position);
    EntityID = -1;
}
