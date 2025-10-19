// ShaderManager.cpp
// 着色器管理类
// created by FirstRay2001, Oct/16/2025

#include "ShaderManager.h"
#include "Source/Scene/Component/Graphic/Shader.h"

const std::string ShaderDirectory = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Shader/";

void MShaderManager::Initialize()
{
}

int MShaderManager::LoadShader(const char* ShaderName, std::string VertexPath, std::string FragmentPath)
{
	// 是否已加载
	int ShaderIndex = FindShader(ShaderName);
	if (ShaderIndex != -1)
		return ShaderIndex;

	// 新建Shader
	int NewShaderID = Shaders_.Size();
	Shaders_.emplace_back(new FShader((ShaderDirectory + VertexPath).c_str(), (ShaderDirectory + FragmentPath).c_str()));
	ShaderMap_[ShaderName] = NewShaderID;
	return NewShaderID;
}

int MShaderManager::FindShader(const char* ShaderName) const
{
	if (ShaderMap_.Find(ShaderName) != nullptr)
		return ShaderMap_[ShaderName];
	else
		return -1;
}

MySTL::TSharedPtr<FShader> MShaderManager::GetShader(int ShaderID)
{
	return Shaders_[ShaderID];
}

MySTL::TSharedPtr<FShader> MShaderManager::GetShader(const char* ShaderName)
{
	int ShaderIndex = FindShader(ShaderName);

	if (ShaderIndex != -1)
		return Shaders_[ShaderIndex];
	else
	{
		LOG_ERROR("Shader not found: %s", ShaderName);
		return MySTL::TSharedPtr<FShader>();
	}
}
