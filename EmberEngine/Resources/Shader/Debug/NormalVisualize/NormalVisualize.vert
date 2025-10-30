#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 5) in ivec4 aBoneIds;
layout (location = 6) in vec4 aWeights;


out VS_OUT
{
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    // 蒙皮网格动画
    mat4 boneTransform = mat4(0.0);

    // 无骨骼的网格
    if(aBoneIds[0] == -1)
        boneTransform = mat4(1.0);

    for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if(aBoneIds[i] != -1)
        {
            boneTransform += aWeights[i] * finalBonesMatrices[aBoneIds[i]];
        }
    }

    gl_Position = view * model * boneTransform * vec4(aPos, 1);
    mat3 normalMatrix = mat3(transpose(inverse(view * model * boneTransform)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}