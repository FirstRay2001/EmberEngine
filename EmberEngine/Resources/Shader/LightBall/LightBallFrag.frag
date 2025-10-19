#version 330 core

out vec4 FragColor;

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

uniform vec3 cameraPos;

void main()
{
    // 衰减
    float distance    = length(cameraPos - pointLight.position);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance) + 0.00001);
    attenuation = attenuation * 0.25 + 0.75;

    FragColor = vec4(attenuation, attenuation, attenuation, 1.0); 
}