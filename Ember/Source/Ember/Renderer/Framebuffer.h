// Framebuffer.h
// 帧缓冲
// created by FirstRay2001, Nov/11/2025

#pragma once

namespace Ember
{
	// 帧缓冲规格
	struct FramebufferSpecification
	{
		uint32_t Width = 1280;
		uint32_t Height = 720;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}

