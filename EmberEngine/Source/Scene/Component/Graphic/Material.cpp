// Material.cpp
// 材质类
// created by FirstRay2001, Oct/15/2025

#include "Material.h"
#include "Shader.h"
#include "Source/Manager/TextureManager.h"

FMaterial::FMaterial() :
	DiffuseTextureName_(""),
	SpecularTextureName_(""),
	Shininess_(32.0f)
{
}

FMaterial::FMaterial(std::string DiffuseTexturName, std::string SpecularTextureName, float Shininess) :
	DiffuseTextureName_(DiffuseTexturName.c_str()),
	SpecularTextureName_(SpecularTextureName.c_str()),
	Shininess_(Shininess)
{
	MTextureManager::GetInstance().LoadTexture(DiffuseTexturName);
	MTextureManager::GetInstance().LoadTexture(SpecularTextureName);
}

void FMaterial::ApplyTo(const FShader& Shader) const
{
	// 设置漫反射贴图
	MTextureManager::GetInstance().BindSampler(DiffuseTextureName_, Shader, "material.diffuse");

	// 设置镜面反射贴图
	MTextureManager::GetInstance().BindSampler(SpecularTextureName_, Shader, "material.specular");

	// 设置高光系数
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

bool FMaterial::operator==(const FMaterial& Other) const
{
	return (DiffuseTextureName_ == Other.DiffuseTextureName_) &&
		(SpecularTextureName_ == Other.SpecularTextureName_) &&
		(Shininess_ == Other.Shininess_);
}
