#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;
layout (location = 5) in ivec4 aBoneIds;
layout (location = 6) in vec4 aWeights;

out VS_OUT
{
    vec3 position;
    vec3 direction;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    int Id = 0;
    if(aBoneIds.x != -1)
    {
        Id = aBoneIds.x;
    }
    if(aBoneIds.y != -1)
    {
        Id = aBoneIds.y;
    }
    if(aBoneIds.z != -1)
    {
        Id = aBoneIds.z;
    }
    if(aBoneIds.w != -1)
    {
        Id = aBoneIds.w;
    }
}