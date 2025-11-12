// OpenGLShader.h
// OpenGL着色器类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Ember
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual const std::string& GetName() const override { return m_Name; }

		virtual void SetUniformInt(const std::string& name, int value) const override;
		virtual void SetUniformFloat(const std::string& name, float value) const override;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) const override;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& vector) const override;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& vector) const override;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) const override;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSrc);

		void OpenGLShader::CompileImpl_Vert_Frag(const std::unordered_map<GLenum, std::string>& shaderSrc);

	private:
		std::string m_Filepath;
		std::string m_Name;
		uint32_t m_RendererID;
	};
}
