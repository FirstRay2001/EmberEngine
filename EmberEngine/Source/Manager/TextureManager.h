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

	// 绑定纹理采样器
	void BindSampler(std::string TextureName, const FShader& Shader, const char* UniformName);

private:
	MTextureManager() = default;

private:
	// 纹理数量
	int TextureCount_ = 0;

	// 已使用纹理单元数量
	int UsedTextureUnitCount_ = 0;

	// 文件名称-纹理ID映射表
	MySTL::THashTable<std::string, GLuint> TextureMap_;
};
