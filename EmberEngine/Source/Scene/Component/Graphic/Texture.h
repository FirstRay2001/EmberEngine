// Texture.h
// 纹理类
// created by FirstRay2001, Sep/16/2025

#pragma once

#include "Common.h"

class FTexture
{
public:
	// 从文件读取图片
	FTexture(const char* PicturePath);
	virtual ~FTexture();

	// 绑定纹理
	void Bind(unsigned int Sampler) const;

private:
	unsigned int	ID_;
	unsigned char*	Data_;
	int				TextureWidth_;
	int				TextureHeight_;
	int				TextureChannelNum_;
};