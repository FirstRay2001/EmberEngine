// Material.h
// 材质类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

namespace Ember
{
	Material::Material(const std::string& name) :
		m_Name(name)
	{
	}

	void Material::ApplyToShader(const Ref<Shader>& shader) const
	{
		// 通用参数
		shader->SetUniformFloat3("u_Material.Albedo", m_Albedo);
		shader->SetUniformInt("u_Material.HasAlbedoTexture", m_AlbedoTexture ? 1 : 0);
		if (m_AlbedoTexture)
		{
			shader->SetUniformInt("u_Material.AlbedoTexture", 0);
			m_AlbedoTexture->Bind(0); // 绑定到纹理单元0
		}

		shader->SetUniformInt("u_Material.HasNormalMap", m_NormalMap ? 1 : 0);
		if (m_NormalMap)
		{
			shader->SetUniformInt("u_Material.NormalTexture", 5);
			m_NormalMap->Bind(5); // 绑定到纹理单元5
		}

		shader->SetUniformFloat3("u_Material.Emissive", m_Emissive);
		shader->SetUniformInt("u_Material.HasEmissiveTexture", m_EmissiveTexture ? 1 : 0);
		if (m_EmissiveTexture)
		{
			shader->SetUniformInt("u_Material.EmissiveTexture", 1);
			m_EmissiveTexture->Bind(1); // 绑定到纹理单元1
		}

		shader->SetUniformFloat("u_Material.Opacity", m_Opacity);

		// Phong/Blinn-Phong参数
		if (m_ShadingModel == ShadingModel::BlinnPhong)
		{
			shader->SetUniformFloat3("u_Material.SpecularColor", m_SpecularColor);
			shader->SetUniformInt("u_Material.HasSpecularTexture", m_SpecularTexture ? 1 : 0);
			if (m_SpecularTexture)
			{
				shader->SetUniformInt("u_Material.SpecularTexture", 2);
				m_SpecularTexture->Bind(2); // 绑定到纹理单元2
			}

			shader->SetUniformFloat("u_Material.Shininess", m_Shininess);
			shader->SetUniformInt("u_Material.HasShininessTexture", m_ShininessTexture ? 1 : 0);
			if (m_ShininessTexture)
			{
				shader->SetUniformInt("u_Material.ShininessTexture", 3);
				m_ShininessTexture->Bind(3); // 绑定到纹理单元3
			}
		}

		// PBR参数
		else if (m_ShadingModel == ShadingModel::PBR)
		{
			shader->SetUniformFloat3("u_Material.SpecularColor", m_SpecularColor);
			shader->SetUniformFloat("u_Material.Glossiness", m_Glossiness);
			shader->SetUniformInt("u_Material.HasSpecularGlossinessTexture", m_SpecularGlossinessTexture ? 1 : 0);
			if (m_SpecularGlossinessTexture)
			{
				shader->SetUniformInt("u_Material.SpecularGlossinessTexture", 4);
				m_SpecularGlossinessTexture->Bind(4); // 绑定到纹理单元4
			}
		}
	}

	Ref<Material> Material::Create(const std::string& name)
	{
		return CreateRef<Material>(name);
	}
}