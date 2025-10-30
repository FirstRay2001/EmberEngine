// TextureManager.h
// 纹理管理器
// created by FirstRay2001, Oct/15/2025

#pragma once

#include "Common.h"

class MTextureManager
{
public:
	// 单例
	static MTextureManager& GetInstance()
	{
		static MTextureManager Instance;
		return Instance;
	}

public:
	// 初始化
	void Initialize();

	// 每帧调用
	void Tick();

	// 清除纹理单元绑定
	void Clear();

public:
	// 加载纹理, 传入纹理名称，返回是否加载成功
	bool LoadTexture(std::string TextureName);

	// 从内存加载纹理, 传入纹理数据、宽高和纹理名称，返回是否加载成功
	bool LoadTextureFromMemory(std::string TextureName, const unsigned char* Data, int DataSize);

	// 储存纹理
	bool StoreTexture(std::string TextureName, GLuint TextureID, GLint TextureType = GL_TEXTURE_2D);

	// 加载CubeMap纹理, 传入纹理名称数组，返回是否加载成功
	bool LoadCubeTexture(const MySTL::TVector<std::string>& TextureNames, std::string CubeMapName);

	// 绑定纹理采样器
	void BindSampler(std::string TextureName, const FShader& Shader, const char* UniformName);

private:
	MTextureManager() = default;

private:
	// 已使用纹理单元数量
	int UsedTextureUnitCount_ = 0;

	// 纹理映射表
	MySTL::THashTable<std::string, size_t> TextureMap_;

	// 纹理数组
	MySTL::TVector<MySTL::TSharedPtr<FTexture>> Textures_;
};
