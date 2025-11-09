// Material.h
// 材质类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Ember
{
	Material::Material(const std::string& name) :
		m_Name(name)
	{
	}

	void Material::ApplyToShader(const Ref<Shader>& shader_) const
	{
		// 类型转化
		auto shader = OPENGLSHADER(shader_);

		// 通用参数
		shader->SetUniformFloat3("u_Material.albedo", m_Albedo);
		shader->SetUniformInt("u_Material.hasAlbedoTexture", m_AlbedoTexture ? 1 : 0);
		if (m_AlbedoTexture)
			m_AlbedoTexture->Bind(0); // 绑定到纹理单元0

		shader->SetUniformInt("u_Material.hasNormalMap", m_NormalMap ? 1 : 0);
		if (m_NormalMap)
			m_NormalMap->Bind(5); // 绑定到纹理单元5

		shader->SetUniformFloat3("u_Material.emissive", m_Emissive);
		shader->SetUniformInt("u_Material.hasEmissiveTexture", m_EmissiveTexture ? 1 : 0);
		if (m_EmissiveTexture)
			m_EmissiveTexture->Bind(1); // 绑定到纹理单元1

		shader->SetUniformFloat("u_Material.opacity", m_Opacity);

		// Phong/Blinn-Phong参数
		if (m_ShadingModel == ShadingModel::BlinnPhong)
		{
			shader->SetUniformFloat3("u_Material.specularColor", m_SpecularColor);
			shader->SetUniformInt("u_Material.hasSpecularTexture", m_SpecularTexture ? 1 : 0);
			if (m_SpecularTexture)
				m_SpecularTexture->Bind(2); // 绑定到纹理单元2

			shader->SetUniformFloat("u_Material.shininess", m_Shininess);
			shader->SetUniformInt("u_Material.hasShininessTexture", m_ShininessTexture ? 1 : 0);
			if (m_ShininessTexture)
				m_ShininessTexture->Bind(3); // 绑定到纹理单元3
		}

		// PBR参数
		else if (m_ShadingModel == ShadingModel::PBR)
		{
			shader->SetUniformFloat3("u_Material.specularColor", m_SpecularColor);
			shader->SetUniformFloat("u_Material.glossiness", m_Glossiness);
			shader->SetUniformInt("u_Material.hasSpecularGlossinessTexture", m_SpecularGlossinessTexture ? 1 : 0);
			if (m_SpecularGlossinessTexture)
				m_SpecularGlossinessTexture->Bind(4); // 绑定到纹理单元4
		}
	}

	Ref<Material> Material::Create(const std::string& name)
	{
		return CreateRef<Material>(name);
	}
}