// Material.h
// 材质类
// created by FirstRay2001, Oct/15/2025

#pragma once

#include "Common.h"

class FMaterial
{
public:
	FMaterial();
	FMaterial(std::string DiffuseTexturName, std::string SpecularTextureName, float Shininess);
	~FMaterial() = default;

public:
	// 应用材质到Shader
	void ApplyTo(const FShader& Shader) const;

	std::string GetDiffuseTexturePath() const;
	std::string GetSpecularTexturePath() const;

public:
	bool operator==(const FMaterial& Other) const;

private:
	std::string DiffuseTextureName_;
	std::string SpecularTextureName_;
	float Shininess_;
};
