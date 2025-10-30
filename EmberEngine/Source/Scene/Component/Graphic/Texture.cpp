// Texture.cpp
// ������
// created by FirstRay2001, Sep/16/2025

#include "Texture.h"

FTexture::FTexture(GLuint ID, GLint TextureType) :
	ID_(ID),
	TextureType_(TextureType)
{
}

FTexture::~FTexture()
{
	// �ͷ�����
	glDeleteTextures(1, &ID_);
}

void FTexture::Bind(unsigned int TextureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + TextureUnit);
	glBindTexture(TextureType_, ID_);
}
