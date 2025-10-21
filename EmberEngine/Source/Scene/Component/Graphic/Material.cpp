// Material.cpp
// ������
// created by FirstRay2001, Oct/15/2025

#include "Material.h"
#include "Shader.h"
#include "Source/Manager/TextureManager.h"

FMaterial::FMaterial() :
	DiffuseTextureName_(),
	SpecularTextureName_(),
	NormalMapName_(),
	Shininess_(32.0f)
{
}

void FMaterial::SetDiffuse(std::string NewDiffuseName)
{
	if (MTextureManager::GetInstance().LoadTexture(NewDiffuseName))
	{
		LOG_INFO("succeed to set diffuse texture:%s", NewDiffuseName.c_str());
		DiffuseTextureName_ = NewDiffuseName;
	}
	else
	{
		LOG_ERROR("failed to set diffuse texture:%s", NewDiffuseName.c_str());
	}
}

void FMaterial::SetSpecular(std::string NewSpecularName)
{
	if (MTextureManager::GetInstance().LoadTexture(NewSpecularName))
	{
		LOG_INFO("succeed to set specular texture:%s", NewSpecularName.c_str());
		DiffuseTextureName_ = NewSpecularName;
	}
	else
	{
		LOG_ERROR("failed to set specular texture:%s", NewSpecularName.c_str());
	}
}

void FMaterial::SetNormal(std::string NewNormalName)
{
	if (MTextureManager::GetInstance().LoadTexture(NewNormalName))
	{
		LOG_INFO("succeed to set normal map:%s", NewNormalName.c_str());
		NormalMapName_ = NewNormalName;
	}
	else
	{
		LOG_ERROR("failed to set normal map:%s", NewNormalName.c_str());
	}
}

void FMaterial::SetShininess(float NewShininess)
{
	Shininess_ = NewShininess;
}



void FMaterial::ApplyTo(const FShader& Shader) const
{
	// ������������ͼ
	MTextureManager::GetInstance().BindSampler2D(DiffuseTextureName_, Shader, "material.diffuse");

	// ���þ��淴����ͼ
	MTextureManager::GetInstance().BindSampler2D(SpecularTextureName_, Shader, "material.specular");

	// ���÷�����ͼ
	MTextureManager::GetInstance().BindSampler2D(NormalMapName_, Shader, "material.normalMap");

	// ���ø߹�ϵ��
	Shader.Use();
	Shader.SetFloat("material.shininess", Shininess_);
}

std::string FMaterial::GetDiffuseTexturePath() const
{
	return DiffuseTextureName_;
}

std::string FMaterial::GetSpecularTexturePath() const
{
	return SpecularTextureName_;
}

std::string FMaterial::GetNormalMapPath() const
{
	return NormalMapName_;
}

bool FMaterial::operator==(const FMaterial& Other) const
{
	return (DiffuseTextureName_ == Other.DiffuseTextureName_) &&
		(SpecularTextureName_ == Other.SpecularTextureName_) &&
		(Shininess_ == Other.Shininess_);
}
