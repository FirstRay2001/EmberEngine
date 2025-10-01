// Texture.cpp
// ������
// created by FirstRay2001, Sep/16/2025

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

FTexture::FTexture(const char* PicturePath)	:
	ID_(),
	Data_(),
	TextureWidth_(),
	TextureHeight_(),
	TextureChannelNum_()
{
	stbi_set_flip_vertically_on_load(true);
	// ��ȡͼƬ
	Data_.Reset(stbi_load(PicturePath, &TextureWidth_, &TextureHeight_, &TextureChannelNum_, 0));

	// ����ID
	glGenTextures(1, &ID_);
	glBindTexture(GL_TEXTURE_2D, ID_);

	// ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth_, TextureHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data_.Get());
	glGenerateMipmap(GL_TEXTURE_2D);
}