// Texture.h
// ������
// created by FirstRay2001, Sep/16/2025

#pragma once

#include "Common.h"

class FTexture
{
public:
	// ���ļ���ȡͼƬ
	FTexture(const char* PicturePath);

private:
	unsigned int	ID_;
	unsigned char*	Data_;
	int				TextureWidth_;
	int				TextureHeight_;
	int				TextureChannelNum_;
};