// Texture.cpp
// 纹理类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "stb_image.h"

namespace Ember
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		// 根据API类型创建纹理实例
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const unsigned char* data, int width, int height, int channel)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(data, width, height, channel);
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	unsigned char* Texture::ReadData(const std::string& path, int& width, int& height, int& channel)
	{
		stbi_set_flip_vertically_on_load(1);
		return stbi_load(path.c_str(), &width, &height, &channel, 0);
	}

	void Texture::freeImageData(unsigned char* data)
	{
		stbi_image_free(data);
	}

	std::string Texture::FilePathToName(const std::string& filepath)
	{
		// 从文件路径中提取名称
		size_t lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = filepath.rfind('.');
		size_t count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		return filepath.substr(lastSlash, count);
	}

	Ref<CubemapTexture> CubemapTexture::Create(const std::array<std::string, 6>& faces)
	{
		// 根据API类型创建立方体纹理实例
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLCubemapTexture>(faces);
		}
		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<CubemapTexture> CubemapTexture::Create(const std::string& directory)
	{
		std::array<std::string, 6> faces =
		{
			directory + "/right.jpg",
			directory + "/left.jpg",
			directory + "/top.jpg",
			directory + "/bottom.jpg",
			directory + "/front.jpg",
			directory + "/back.jpg"
		};
		return Create(faces);
	}

}