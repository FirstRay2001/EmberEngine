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

	private:
		uint32_t m_RendererID;
	};
}
