// Material.h
// 材质类
// created by FirstRay2001, Oct/15/2025

#pragma once

#include "Common.h"

class FMaterial
{
public:
	FMaterial();
	~FMaterial() = default;

public:
	void SetDiffuse(std::string NewDiffuseName);
	void SetSpecular(std::string NewSpecularName);
	void SetNormal(std::string NewNormalName);
	void SetShininess(float NewShininess);

	// 应用材质到Shader
	void ApplyTo(const FShader& Shader) const;

	std::string GetDiffuseTexturePath() const;
	std::string GetSpecularTexturePath() const;
	std::string GetNormalMapPath() const;

public:
	bool operator==(const FMaterial& Other) const;

private:
	std::string DiffuseTextureName_;
	std::string SpecularTextureName_;
	std::string NormalMapName_;
	float Shininess_;
};
