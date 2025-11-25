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
		virtual void Init() override;
		virtual void* SetupMutiThread(void* window) override;
		virtual void InitMutiThread(void* window) override;
		virtual void ShutdownMutiThread() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray) override;
		virtual void SetViewport(const uint32_t& width, const uint32_t& height) override;
		virtual void SetDepthMask(bool enabled) override;
		virtual void SetCullFace(CullFace face) override;

		// 模板测试相关函数
		virtual void EnableStencilTest(bool enabled) override;
		virtual void SetStencilFunc(RendererAPI::StencilFunc func, int ref, uint32_t mask) override;
		virtual void SetStencilOp(RendererAPI::StencilOp sfail, RendererAPI::StencilOp dpfail, RendererAPI::StencilOp dppass) override;
		virtual void SetStencilMask(unsigned int mask) override;
	};
}


