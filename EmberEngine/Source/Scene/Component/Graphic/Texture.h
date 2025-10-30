// Texture.h
// ������
// created by FirstRay2001, Sep/16/2025

#pragma once

#include "Common.h"

class FTexture
{
public:
	// ���ļ���ȡͼƬ
	FTexture(GLuint ID, GLint TextureType);
	virtual ~FTexture();

	// ������
	void Bind(unsigned int TextureUnit) const;

private:
	unsigned int	ID_;
	int				TextureType_;
};