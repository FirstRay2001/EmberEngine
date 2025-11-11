// OpenGLFramebuffer.h
// OpenGL帧缓冲
// created by FirstRay2001, Nov/11/2025

#pragma once

#include "Ember/Renderer/FrameBuffer.h"

namespace Ember
{
	class OpenGlFrameBuffer : public Framebuffer
	{
	public:
		OpenGlFrameBuffer(const FramebufferSpecification& spec);
		virtual ~OpenGlFrameBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};
}