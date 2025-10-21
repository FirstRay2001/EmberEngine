#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoord;
out mat3 TBN;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // 顶点世界坐标
    WorldPos = vec3(model * vec4(aPos, 1));

    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;

    // TBN矩阵
    vec3 NormalWorld = (model * vec4(aNormal, 0)).xyz;
	vec3 TangentWorld = (model * vec4(aTangent, 0)).xyz;
	vec3 BiTangentWorld = (model * vec4(aBiTangent, 0)).xyz;
	TBN = mat3(TangentWorld, BiTangentWorld, NormalWorld);

    // 光源空间矩阵
    FragPosLightSpace = lightSpaceMatrix * vec4(WorldPos, 1);
}