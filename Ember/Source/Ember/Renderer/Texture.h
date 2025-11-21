// Texture.h
// 纹理类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Core/Core.h"

namespace Ember
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		// 获取图像路径
		virtual const std::string& GetPath() const = 0;

		// 文件路径转换为名称
		static std::string FilePathToName(const std::string& filepath);
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const unsigned char* data, int width, int height, int channel);

		// 读取图像到内存
		static unsigned char* ReadData(const std::string& path, int& width, int& height, int& channel);

		// 释放图像内存
		static void freeImageData(unsigned char* data);
	};
}
