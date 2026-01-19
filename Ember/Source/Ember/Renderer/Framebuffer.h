// Framebuffer.h
// 帧缓冲
// created by FirstRay2001, Nov/11/2025

#pragma once

namespace Ember
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// 颜色格式
		RGBA8,
		RED_INTEGER,

		// 深度/模板格式
		DEPTH24STENCIL8,

		// 默认深度格式
		Depth = DEPTH24STENCIL8
	};

	// 帧缓冲纹理规格
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {
		}
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	// 帧缓冲附件规格
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {
		}
		std::vector<FramebufferTextureSpecification> Attachments;
	};

	// 帧缓冲规格
	struct FramebufferSpecification
	{
		uint32_t Width = 1280;
		uint32_t Height = 720;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t width, uint32_t heihgt) = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) const = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		static const Framebuffer* GetCurrentFramebuffer() { return s_CurrentFramebuffer; }

	protected:
		static const Framebuffer* s_CurrentFramebuffer;
	};
}

