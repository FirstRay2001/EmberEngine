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
    vec3 Albedo;
    vec3 SpecularColor;
    float Shininess;
    vec3 Emissive;
    float Opacity;
    
    // 纹理标志
    bool HasAlbedoTexture;
    bool HasSpecularTexture;
	bool HasShininessTexture;
    bool HasNormalTexture;
    bool HasEmissiveTexture;

	// 纹理
	sampler2D AlbedoTexture;
	sampler2D EmissiveTexture;
	sampler2D SpecularTexture;
	sampler2D ShininessTexture;
	sampler2D NormalTexture;
};

struct Camera
{
	vec3 Position;
	vec3 Direction;
};

struct PointLight
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Constant;
	float Quadratic;
};

struct DirectionalLight
{
	vec3 Direction;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
}

uniform Material u_Material;
uniform Camera u_Camera;
uniform PointLight u_PointLights[4];
uniform DirectionalLight u_DirectionalLight;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

out vec4 FragColor;

void main() {
	// 基础色
    vec3 albedo = u_Material.Albedo;
    if (u_Material.HasAlbedoTexture)
	{
        albedo *= texture(u_Material.AlbedoTexture, v_TexCoord).rgb;
    }
	
    // 高光色
    vec3 specularColor = u_Material.SpecularColor;
    if (u_Material.HasSpecularTexture)
	{
        specularColor *= texture(u_Material.SpecularTexture, v_TexCoord).rgb;
    }

	// 光泽度
	float shininess = u_Material.Shininess;
	if (u_Material.HasShininessTexture)
	{
		shininess = texture(u_Material.ShininessTexture, v_TexCoord).r; 
	}
    
	// 法线
    vec3 normal = normalize(v_Normal);
    if (u_Material.HasNormalTexture) {
        normal = texture(u_Material.NormalTexture, v_TexCoord).rgb * 2.0 - 1.0;
        normal = normalize(normal);
    }
    
	vec3 L = -1.0 * u_DirectionalLight.Direction;
	vec3 N = v_Normal;
	vec3 V = normalize(u_Camera.Position - v_FragPos);
	vec3 H = normalize(L + V);

	vec3 ambient = albedo * u_DirectionalLight.Ambient;
	vec3 diffuse = albedo * clamp(dot(N, L), 0, 1) * u_DirectionalLight.Diffuse;
	vec3 specular = specularColor * pow(clamp(dot(N, H), 0, 1), u_Material.Shininess) * u_DirectionalLight.Specular;

    vec3 result = ambient + diffuse + specular;
	
    FragColor = vec4(result, 1);
}