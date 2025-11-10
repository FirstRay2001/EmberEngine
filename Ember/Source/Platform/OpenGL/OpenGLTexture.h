// OpenGLTexture.h
// OpenGL纹理类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Renderer/Texture.h"

namespace Ember
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const unsigned char* data, int width, int height, int channel);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) const override;
		virtual int GetWidth() const override { return m_Width; }
		virtual int GetHeight() const override { return m_Height; }
	private:
		uint32_t m_RendererID;
		std::string m_Path;
		int m_Width, m_Height, m_Channels;
	};
}


