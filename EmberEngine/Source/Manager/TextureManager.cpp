// TextureManager.cpp
// ���������
// created by FirstRay2001, Oct/15/2025

#include "TextureManager.h"
#include "Source/Scene/Component/Graphic/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const std::string TextureDirectory = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/";

void MTextureManager::Initialize()
{
}

void MTextureManager::Tick()
{
	// ÿһ֡������ʹ������Ԫ����
	Clear();
}

void MTextureManager::Clear()
{
	UsedTextureUnitCount_ = 0;
}

bool MTextureManager::LoadTexture(std::string TextureName)
{
	// �����Ѽ���
	if (TextureMap_.Find(TextureName) != nullptr)
	{
		LOG_WARN("Texture already loaded: %s", TextureName.c_str());
		return true;
	}

	// ���·��ת��Ϊ����·��
	// std::string PicPath = TextureDirectory + TextureName;

	// ��ֱ��תͼƬ
	stbi_set_flip_vertically_on_load(true);

	// ��ȡͼƬ
	int PicWidth, PicHeight, PicChannelNum;
	auto* Data = stbi_load(TextureName.c_str(), &PicWidth, &PicHeight, &PicChannelNum, 0);

	// ����Ƿ��ȡ�ɹ�
	if (Data == NULL)
	{
		LOG_ERROR("Failed to load texture: %s", TextureName.c_str());
		return false;
	}
	LOG_INFO("Texture loaded: %s (Width: %d, Height: %d, Channels: %d)",
		TextureName.c_str(), PicWidth, PicHeight, PicChannelNum);
	
	// ��������
	GLuint TextureID;
	glGenTextures(1, &TextureID);

	// ������
	glBindTexture(GL_TEXTURE_2D, TextureID);

	// ����ͼƬ����
	GLenum Format;
	if (PicChannelNum == 1)
		Format = GL_RED;
	else if (PicChannelNum == 3)
		Format = GL_RGB;
	else if (PicChannelNum == 4)
		Format = GL_RGBA;
	else
	{
		LOG_ERROR("Unsupported texture format: %s", TextureName.c_str());
		stbi_image_free(Data);
		return false;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, Format, PicWidth, PicHeight, 0, Format, GL_UNSIGNED_BYTE, Data);

	// �����������
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// �洢����Ԫ����
	TextureMap_.Insert(TextureName, TextureID);
	TextureCount_++;

	// �ͷ�ͼƬ�ڴ�
	stbi_image_free(Data);
}

void MTextureManager::BindSampler(std::string TextureName, const FShader& Shader, const char* UniformName)
{
	auto It = TextureMap_.Find(TextureName);

	// ��������
	if (It == nullptr)
	{
		LOG_ERROR("Texture not found: %s", TextureName.c_str());
		return;
	}

	GLuint TextureID = *It;

	// ��������Ԫ
	glActiveTexture(GL_TEXTURE0 + UsedTextureUnitCount_);

	// ������
	glBindTexture(GL_TEXTURE_2D, TextureID);

	// ���ò�����
	Shader.SetInt(UniformName, UsedTextureUnitCount_);

	UsedTextureUnitCount_++;
}
