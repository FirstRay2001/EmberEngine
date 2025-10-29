#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;
layout (location = 5) in ivec4 aBoneIds;
layout (location = 6) in vec4 aWeights;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoord;
out mat3 TBN;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    // 顶点世界坐标
    WorldPos = vec3(model * vec4(aPos, 1));

    gl_Position = projection * view * vec4(WorldPos, 1.0);

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