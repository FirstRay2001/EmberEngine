// OpenGLRendererAPI.h
// OpenGL渲染API类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Renderer/RendererAPI.h"

namespace Ember
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void SetViewport(const uint32_t& width, const uint32_t& height) override;
	};
}


