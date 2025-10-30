// Texture.h
// 纹理类
// created by FirstRay2001, Sep/16/2025

#pragma once

#include "Common.h"

class FTexture
{
public:
	// 从文件读取图片
	FTexture(GLuint ID, GLint TextureType);
	virtual ~FTexture();

	// 绑定纹理
	void Bind(unsigned int TextureUnit) const;

private:
	unsigned int	ID_;
	int				TextureType_;
};