// Texture.cpp
// 纹理类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

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
}