// ShaderManager.h
// 着色器管理器
// created by FirstRay2001, Oct/16/2025

#pragma once

#include "Common.h"

class MShaderManager
{
public:
	// 单例
	static MShaderManager& GetInstance()
	{
		static MShaderManager Instance;
		return Instance;
	}

public:
	void Initialize();

public:
	// 加载Shader, 传入Shader名称、顶点着色器路径、片段着色器路径，返回Shader ID
	int LoadShader(const char* ShaderName, std::string VertexPath, std::string FragmentPath);

	// 查找Shader，传入Shader名称，返回Shader ID，未找到返回-1
	int FindShader(const char* ShaderName) const;

	// 获取Shader，传入Shader ID，返回智能指针
	MySTL::TSharedPtr<FShader> GetShader(int ShaderID);

	// 获取Shader，传入Shader名称，返回智能指针
	MySTL::TSharedPtr<FShader> GetShader(const char* ShaderName);

private:
	MShaderManager() = default;

	MySTL::THashTable<const char*, int>			ShaderMap_;
	MySTL::TVector<MySTL::TSharedPtr<FShader>>	Shaders_;
};
