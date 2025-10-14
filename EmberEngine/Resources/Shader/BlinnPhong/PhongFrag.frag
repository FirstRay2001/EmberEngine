#version 330 core

out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 objectColor;

void main()
{
    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * objectColor;

    // 漫反射
    vec3 L = normalize(lightPos - WorldPos);
    vec3 N = normalize(Normal);
    float NL = max(dot(N, L), 0);
    vec3 diffuse = NL * lightColor * objectColor;

    // 镜面反射
    float specularStrength = 0.5;
    vec3 V = normalize(cameraPos - WorldPos);
    vec3 H = normalize(V + L);
    float NH = max(dot(N, H), 0);
    vec3 specular = specularStrength * pow(NH, 32) * lightColor;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}