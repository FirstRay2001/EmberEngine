// TextureManager.cpp
// ���������
// created by FirstRay2001, Oct/15/2025

#include "TextureManager.h"
#include "Source/Scene/Component/Graphic/Shader.h"
#include "Source/Scene/Component/Graphic/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const std::string TextureDirectory = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/";
const std::string FullyBlackTexture = "fully_black.png";

void MTextureManager::Initialize()
{
	LoadTexture(TextureDirectory + FullyBlackTexture);
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
	TextureMap_.Insert(TextureName, Textures_.Size());
	Textures_.emplace_back(new FTexture(TextureID, GL_TEXTURE_2D));

	// �ͷ�ͼƬ�ڴ�
	stbi_image_free(Data);

	return true;
}

bool MTextureManager::LoadTextureFromMemory(std::string TextureName, const unsigned char* CompressedData, int DataSize)
{
	// �����Ѽ���
	if (TextureMap_.Find(TextureName) != nullptr)
	{
		LOG_WARN("Texture already loaded: %s", TextureName.c_str());
		return true;
	}

	// ��������
	GLuint TextureID;
	glGenTextures(1, &TextureID);

	// ������
	glBindTexture(GL_TEXTURE_2D, TextureID);

	// ��������
	int Width, Height, ChannelNum;
	unsigned char* Data = stbi_load_from_memory(CompressedData, DataSize, &Width, &Height, &ChannelNum, 4);

	// ����ͼƬ����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);

	// �����������
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// �洢����Ԫ����
	TextureMap_.Insert(TextureName, Textures_.Size());
	Textures_.emplace_back(new FTexture(TextureID, GL_TEXTURE_2D));

	// �ͷ�ͼƬ�ڴ�
	stbi_image_free(Data);

	return true;
}

bool MTextureManager::StoreTexture(std::string TextureName, GLuint TextureID, GLint TextureType)
{
	// �洢����Ԫ����
	TextureMap_.Insert(TextureName, Textures_.Size());
	Textures_.emplace_back(new FTexture(TextureID, TextureType));

	return true;
}

bool MTextureManager::LoadCubeTexture(const MySTL::TVector<std::string>& TextureNames, std::string CubeMapName)
{
	// �����Ѽ���
	if (TextureMap_.Find(CubeMapName) != nullptr)
	{
		LOG_WARN("CubeMap texture already loaded: %s", CubeMapName.c_str());
		return true;
	}

	// ��ֱ��תͼƬ
	stbi_set_flip_vertically_on_load(false);

	// ��������
	GLuint TextureID;
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

	// ��ȡͼƬ
	int PicWidth, PicHeight, PicChannelNum;
	for (unsigned int i = 0; i < TextureNames.Size(); i++)
	{
		auto* Data = stbi_load(TextureNames[i].c_str(), &PicWidth, &PicHeight, &PicChannelNum, 0);
		if (Data == NULL)
		{
			LOG_ERROR("Failed to load cube map texture: %s", TextureNames[i].c_str());
			return false;
		}

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
			LOG_ERROR("Unsupported texture format: %s", TextureNames[i].c_str());
			stbi_image_free(Data);
			return false;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Format, PicWidth, PicHeight, 0, Format, GL_UNSIGNED_BYTE, Data);

		// �ͷ�ͼƬ�ڴ�
		stbi_image_free(Data);
	}

	// �����������
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// �洢����Ԫ����
	TextureMap_.Insert(CubeMapName, Textures_.Size());
	Textures_.emplace_back(new FTexture(TextureID, GL_TEXTURE_CUBE_MAP));

	return true;
}

void MTextureManager::BindSampler(std::string TextureName, const FShader& Shader, const char* UniformName)
{
	// ������
	if (TextureName.empty())
	{
		// LOG_WARN("empty texture, use default to %s", UniformName);
		BindSampler(TextureDirectory + FullyBlackTexture, Shader, UniformName);
		return;
	}

	auto It = TextureMap_.Find(TextureName);

	// ��������
	if (It == nullptr)
	{
		LOG_ERROR("Texture not found: %s", TextureName.c_str());
		return;
	}

	// ����������Ԫ
	Textures_[*It]->Bind(UsedTextureUnitCount_);

	// ���ò�����
	Shader.SetInt(UniformName, UsedTextureUnitCount_);

	UsedTextureUnitCount_++;
}