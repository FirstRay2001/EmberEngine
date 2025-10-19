#version 330 core

out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

// 点光源
struct PointLight
{
    vec3 position;  // 位置

    vec3 ambient;   // 环境分量强度
    vec3 diffuse;   // 漫反射分量强度
    vec3 specular;  // 镜面反射分量强度

    float constant;
    float linear;
    float quadratic;

};
uniform PointLight pointLight;

// 平行光
struct DirectionalLight
{
    vec3 direction; // 方向

    vec3 ambient;   // 环境分量强度
    vec3 diffuse;   // 漫反射分量强度
    vec3 specular;  // 镜面反射分量强度
};
uniform DirectionalLight dirLight;

struct Material
{
    sampler2D diffuse;  // 漫反射贴图
    sampler2D specular; // 镜面反射贴图
    float shininess;     // 光泽度
};
uniform Material material;

uniform vec3 cameraPos;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // NDC
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    
    return shadow;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 基础色
    vec3 baseColor = vec3(texture(material.diffuse, TexCoord));

    // 镜面反射强度
    vec3 specularCoef = vec3(texture(material.specular, TexCoord));

    // 环境光
    vec3 ambient = light.ambient * baseColor;

    // 漫反射
    vec3 L = normalize(light.position - fragPos);
    vec3 N = normalize(normal);
    float NL = max(dot(N, L), 0);
    vec3 diffuse = NL * light.diffuse * baseColor;

    // 镜面反射
    vec3 H = normalize(viewDir + L);
    float NH = max(dot(N, H), 0);
    vec3 specular = pow(NH, material.shininess) * light.specular * specularCoef;

    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    vec3 result = (ambient + diffuse + specular) * attenuation;

    return result;
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 基础色
    vec3 baseColor = vec3(texture(material.diffuse, TexCoord));

    // 镜面反射强度
    vec3 specularCoef = vec3(texture(material.specular, TexCoord));

    // 环境光
    vec3 ambient = light.ambient * baseColor;

    // 漫反射
    vec3 L = -normalize(light.direction);
    vec3 N = normalize(normal);
    float NL = max(dot(N, L), 0);
    vec3 diffuse = NL * light.diffuse * baseColor;

    // 镜面反射
    vec3 H = normalize(viewDir + L);
    float NH = max(dot(N, H), 0);
    vec3 specular = pow(NH, material.shininess) * light.specular * specularCoef;

    // 计算阴影
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 result = ambient + (diffuse + specular) * (1 - shadow);

    return result;
}


void main()
{
    //相机向量
    vec3 V = normalize(cameraPos - WorldPos);

    vec3 result = vec3(0);

    // 平行光
    result += CalcDirectionalLight(dirLight, Normal, WorldPos, V);

    // 点光源
    result += CalcPointLight(pointLight, Normal, WorldPos, V);

    FragColor = vec4(result, 1.0);
}