// Shader.cpp
// 着色器类
// created by FirstRay2001, Nov/5/2025

#include "emberpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Ember
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		// 根据当前渲染API创建着色器
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// 根据当前渲染API创建着色器
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::string Shader::FilePathToName(const std::string& filepath)
	{
		// 从文件路径中提取名称
		size_t lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = filepath.rfind('.');
		size_t count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		return filepath.substr(lastSlash, count);
	}
}