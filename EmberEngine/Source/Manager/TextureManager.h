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

	// �����������
	void BindSampler(std::string TextureName, const FShader& Shader, const char* UniformName);

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
