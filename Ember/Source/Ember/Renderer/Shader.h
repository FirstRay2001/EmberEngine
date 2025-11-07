// Shader.h
// 着色器类
// created by FirstRay2001, Nov/5/2025

#pragma once

#include "glm/glm.hpp"

namespace Ember
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		// 绑定着色器程序
		virtual void Bind() const = 0;

		// 解绑着色器程序
		virtual void Unbind() const = 0;

		// 从文件创建着色器
		static Ref<Shader> Create(const std::string& filepath);

		// 从字符串创建着色器
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		// 获取着色器名称
		virtual const std::string& GetName() const = 0;
	};

	// 着色器库
	class ShaderLibrary
	{
	public:
		// 添加着色器
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		// 加载着色器
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		// 获取着色器
		Ref<Shader> Get(const std::string& name);

		// 检查着色器是否存在
		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
