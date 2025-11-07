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

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}
