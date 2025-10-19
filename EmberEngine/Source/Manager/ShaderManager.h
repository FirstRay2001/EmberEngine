// ShaderManager.h
// ��ɫ��������
// created by FirstRay2001, Oct/16/2025

#pragma once

#include "Common.h"

class MShaderManager
{
public:
	// ����
	static MShaderManager& GetInstance()
	{
		static MShaderManager Instance;
		return Instance;
	}

public:
	void Initialize();

public:
	// ����Shader, ����Shader���ơ�������ɫ��·����Ƭ����ɫ��·��������Shader ID
	int LoadShader(const char* ShaderName, std::string VertexPath, std::string FragmentPath);

	// ����Shader������Shader���ƣ�����Shader ID��δ�ҵ�����-1
	int FindShader(const char* ShaderName) const;

	// ��ȡShader������Shader ID����������ָ��
	MySTL::TSharedPtr<FShader> GetShader(int ShaderID);

	// ��ȡShader������Shader���ƣ���������ָ��
	MySTL::TSharedPtr<FShader> GetShader(const char* ShaderName);

private:
	MShaderManager() = default;

	MySTL::THashTable<const char*, int>			ShaderMap_;
	MySTL::TVector<MySTL::TSharedPtr<FShader>>	Shaders_;
};
