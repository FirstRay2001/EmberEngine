// Framebuffer.cpp
// 帧缓冲
// created by FirstRay2001, Nov/11/2025

#include "emberpch.h"
#include "FrameBuffer.h"
#include "Ember/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Ember
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		// 根据渲染API选择具体的帧缓冲实现
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGlFrameBuffer>(spec);
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}