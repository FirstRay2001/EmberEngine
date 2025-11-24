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
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index) const override { return m_ColorAttachments[index]; }
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) const override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
	};
}