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
	bool StoreTexture(std::string TextureName, GLuint TextureID, GLint TextureType = GL_TEXTURE_2D);

	// ����CubeMap����, ���������������飬�����Ƿ���سɹ�
	bool LoadCubeTexture(const MySTL::TVector<std::string>& TextureNames, std::string CubeMapName);

	// �����������
	void BindSampler(std::string TextureName, const FShader& Shader, const char* UniformName);

private:
	MTextureManager() = default;

private:
	// ��ʹ������Ԫ����
	int UsedTextureUnitCount_ = 0;

	// ����ӳ���
	MySTL::THashTable<std::string, size_t> TextureMap_;

	// ��������
	MySTL::TVector<MySTL::TSharedPtr<FTexture>> Textures_;
};
