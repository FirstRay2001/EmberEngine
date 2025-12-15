// 顶点着色器
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in ivec4 a_BoneIDs;
layout(location = 4) in vec4 a_Weights;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 u_BonesMatrices[MAX_BONES];

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_FragPos;

void main()
{
	// 蒙皮网格动画
    mat4 boneTransform = mat4(1.0);

    if(a_BoneIDs[0] != -1)
	{
        boneTransform = mat4(0.0);

		// 累加影响当前Mesh的骨骼权重
		for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if(a_BoneIDs[i] != -1)
			{
				boneTransform += a_Weights[i] * u_BonesMatrices[a_BoneIDs[i]];
			}
		}
	}

	gl_Position = u_ViewProjection * u_Transform * boneTransform * vec4(a_Position, 1.0);

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
	float Linear;
	float Quadratic;
};

struct DirectionalLight
{
	vec3 Direction;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform Material u_Material;
uniform Camera u_Camera;
uniform int u_PointLightCount;
uniform PointLight u_PointLights[4];
uniform DirectionalLight u_DirectionalLight;

uniform int u_EntityID;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;


vec3 CalcDirectionalLighting(vec3 albedo, vec3 specularColor, float shininess, vec3 normal)
{
	vec3 L = -1.0 * u_DirectionalLight.Direction;
	vec3 N = normal;
	vec3 V = normalize(u_Camera.Position - v_FragPos);
	vec3 H = normalize(L + V);

	vec3 ambient = albedo * u_DirectionalLight.Ambient;
	vec3 diffuse = albedo * clamp(dot(N, L), 0, 1) * u_DirectionalLight.Diffuse;
	vec3 specular = specularColor * pow(clamp(dot(N, H), 0, 1), shininess) * u_DirectionalLight.Specular;

    vec3 result = ambient + diffuse + specular;

	return result;
}

vec3 CalcPointLighting(vec3 albedo, vec3 specularColor, float shininess, vec3 normal, PointLight pointLight)
{
    vec3 L = normalize(pointLight.Position - v_FragPos);
    vec3 N = normal;
    vec3 V = normalize(u_Camera.Position - v_FragPos);
    vec3 H = normalize(L + V);

    vec3 ambient = albedo * pointLight.Ambient;
    vec3 diffuse = albedo * clamp(dot(N, L), 0, 1) * pointLight.Diffuse;
    vec3 specular = specularColor * pow(clamp(dot(N, H), 0, 1), shininess) * pointLight.Specular;

    // 光照衰减
    float distance = length(pointLight.Position - v_FragPos);
    float attenuation = 1.0 / (pointLight.Constant + 
                              pointLight.Linear * distance + 
                              pointLight.Quadratic * (distance * distance));

    vec3 result = ambient + (diffuse + specular) * attenuation;
    return result;
}

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
    
	vec3 lighting = vec3(0, 0, 0);

	// 环境光照
	// TODO: 环境光

	// 平行光照
	lighting += CalcDirectionalLighting(albedo, specularColor, shininess, normal);

	// 点光源光照
	for(int i = 0; i < u_PointLightCount; i++)
		lighting += CalcPointLighting(albedo, specularColor, shininess, normal, u_PointLights[i]);
	
    FragColor = vec4(lighting, 1);
	EntityID = u_EntityID;
}