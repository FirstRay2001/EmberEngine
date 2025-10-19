#version 330 core

out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoord;
in mat3 TBN;
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
    sampler2D normalMap; // 法线贴图
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

    float currentDepth = projCoords.z;

    // PCF
    float shadow = 0.0;
    float bias = 0.001;
    vec2 texelSize = 1.0 / vec2(1024, 1024);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
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
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance) + 0.00001); 

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
    vec3 N = Normal;

    // 计算法线
    vec3 SampleNormal = vec3(texture(material.normalMap, TexCoord));
    float NormalLength = length(SampleNormal);
    SampleNormal = SampleNormal *2 - 1;
    N = N * (1 - NormalLength) + TBN * SampleNormal * NormalLength; 

    vec3 result = vec3(0);

    // 平行光
    result += CalcDirectionalLight(dirLight, N, WorldPos, V);

    // 点光源
    result += CalcPointLight(pointLight, N, WorldPos, V);

    // 透明度
    float alpha = texture(material.diffuse, TexCoord).a;

    // result = vec3(dot(N, normalize(pointLight.position - WorldPos)));

    FragColor = vec4(result, alpha);
}