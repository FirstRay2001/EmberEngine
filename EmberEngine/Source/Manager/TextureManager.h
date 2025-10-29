// TextureManager.h
// ���������
// created by FirstRay2001, Oct/15/2025

#pragma once

#include "Common.h"

class MTextureManager
{
public:
	// ����
	static MTextureManager& GetInstance()
	{
		static MTextureManager Instance;
		return Instance;
	}

public:
	// ��ʼ��
	void Initialize();

	// ÿ֡����
	void Tick();

	// �������Ԫ��
	void Clear();

public:
	// ��������, �����������ƣ������Ƿ���سɹ�
	bool LoadTexture(std::string TextureName);

	// ���ڴ��������, �����������ݡ���ߺ��������ƣ������Ƿ���سɹ�
	bool LoadTextureFromMemory(std::string TextureName, const unsigned char* Data, int DataSize);

	// ��������
	bool StoreTexture(std::string TextureName, GLuint TextureID);

	// ����CubeMap����, ���������������飬�����Ƿ���سɹ�
	bool LoadCubeTexture(const MySTL::TVector<std::string>& TextureNames, std::string CubeMapName);

	// ��2D���������
	void BindSampler2D(std::string TextureName, const FShader& Shader, const char* UniformName);

	// ��CubeMap���������
	void BindSamplerCube(std::string CubeMapName, const FShader& Shader, const char* UniformName);

	// ��������ID
	GLuint FindTextureID(std::string TextureName);

private:
	MTextureManager() = default;

private:
	// ��������
	int TextureCount_ = 0;

	// ��ʹ������Ԫ����
	int UsedTextureUnitCount_ = 0;

	// �ļ�����-����IDӳ���
	MySTL::THashTable<std::string, GLuint> TextureMap_;
};
