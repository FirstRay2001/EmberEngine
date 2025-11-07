// OpenGLShader.h
// OpenGL着色器类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Renderer/Shader.h"

namespace Ember
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetUniformInt(const std::string& name, int value) const;

		void SetUniformFloat(const std::string& name, float value) const;
		void SetUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void SetUniformFloat3(const std::string& name, const glm::vec3& vector) const;
		void SetUniformFloat4(const std::string& name, const glm::vec4& vector) const;

		void SetUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const;

	private:
		uint32_t m_RendererID;
	};
}
