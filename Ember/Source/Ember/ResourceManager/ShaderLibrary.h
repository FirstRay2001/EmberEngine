// ShaderLibrary.h
// 着色器管理器
// created by FirstRay2001, Nov/11/2025

#pragma once

#include "Ember/Core/Core.h"
#include "Ember/ResourceManager/ResourceManager.h"

namespace Ember
{
	class Shader;

	// 着色器库
	class ShaderLibrary
	{
	public:
		// 单例
		static ShaderLibrary& Get()
		{
			static ShaderLibrary instance;
			return instance;
		}

	public:
		// 添加着色器
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		// 同步加载着色器
		Ref<Shader> LoadSync(const std::string& filepath);
		Ref<Shader> LoadSync(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		// 异步加载着色器
		void LoadAsync(const std::string& filepath);

		// 立即获取着色器
		Ref<Shader> GetShaderSync(const std::string& name);

		// 非阻塞获取着色器
		Ref<Shader> GetShaderAsync(const std::string& name);

		// 检查着色器是否存在
		bool Exists(const std::string& name) const;

	private:
		ShaderLibrary() = default;
		~ShaderLibrary() = default;

	private:
		std::unordered_map<std::string, std::future<ResourceHandle>>	m_LoadingShaders;
		std::unordered_map<std::string, Ref<Shader>>	m_Shaders;
	};
}


