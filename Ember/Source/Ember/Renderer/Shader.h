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

		virtual void SetUniformInt(const std::string& name, int value) const = 0;
		virtual void SetUniformFloat(const std::string& name, float value) const = 0;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& vector) const = 0;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& vector) const = 0;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) const = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;

		// 文件路径转换为名称
		static std::string FilePathToName(const std::string& filepath);
	};
}
