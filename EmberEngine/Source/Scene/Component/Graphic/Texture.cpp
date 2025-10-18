// Texture.cpp
// 纹理类
// created by FirstRay2001, Sep/16/2025

#include "Texture.h"

FTexture::FTexture(const char* PicturePath)	:
	ID_(),
	Data_(),
	TextureWidth_(),
	TextureHeight_(),
	TextureChannelNum_()
{
	// 创建ID
	glGenTextures(1, &ID_);
	glBindTexture(GL_TEXTURE_2D, ID_);

	// 设置属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 绑定数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth_, TextureHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data_);
	glGenerateMipmap(GL_TEXTURE_2D);
}

FTexture::~FTexture()
{
	delete[] Data_;
}

void FTexture::Bind(unsigned int Sampler) const
{
	
}
