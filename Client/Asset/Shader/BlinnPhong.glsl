// 顶点着色器
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_FragPos;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);

	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_TexCoord = a_TexCoord;
	v_FragPos = vec3(u_Transform * vec4(a_Position, 1));
}


// 片段着色器
#type fragment
#version 330 core

struct Material {
    vec3 albedo;
    vec3 specularColor;
    float shininess;
    vec3 emissive;
    float alpha;
    
    // 纹理标志
    bool hasAlbedoTexture;
    bool hasSpecularTexture;
	bool hasShininessTexture;
    bool hasNormalTexture;
    bool hasEmissiveTexture;

	// 纹理采样器
	sampler2D albedoTexture;
	sampler2D emissiveTexture;
	sampler2D specularTexture;
	sampler2D shininessTexture;
	sampler2D normalTexture;
};

uniform Material u_Material;



in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

out vec4 FragColor;

void main() {
	// 基础色
    vec3 albedo = u_Material.albedo;
    if (u_Material.hasAlbedoTexture)
	{
        albedo *= texture(u_Material.albedoTexture, v_TexCoord).rgb;
    }
	
    // 高光色
    vec3 specularColor = u_Material.specularColor;
    if (u_Material.hasSpecularTexture)
	{
        specularColor *= texture(u_Material.specularTexture, v_TexCoord).rgb;
    }

	// 光泽度
	float shininess = u_Material.shininess;
	if (u_Material.hasShininessTexture)
	{
		shininess = texture(u_Material.shininessTexture, v_TexCoord).r; 
	}
    
	// 法线
    vec3 normal = normalize(v_Normal);
    if (u_Material.hasNormalTexture) {
        normal = texture(u_Material.normalTexture, v_TexCoord).rgb * 2.0 - 1.0;
        normal = normalize(normal);
    }
    
	vec3 ambient = albedo * 0.1;
	vec3 L = normalize(vec3(0, 0, 1));
	vec3 diffuse = albedo * dot(v_Normal, L);

    vec3 result = ambient + diffuse; // 环境光
    
    FragColor = vec4(result, 1);
}