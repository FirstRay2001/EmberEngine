// RenderCommand.h
// 渲染命令
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "RendererAPI.h"

namespace Ember
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void* SetupMutithread(void* window)
		{
			return s_RendererAPI->SetupMutiThread(window);
		}

		inline static void InitMutiThread(void* window)
		{
			s_RendererAPI->InitMutiThread(window);
		}

		inline static void ShutdownMutiThread()
		{
			s_RendererAPI->ShutdownMutiThread();
		}

		// 清除屏幕
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		// 设置清除颜色
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		// 绘制顶点数组
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		// 绘制线条
		inline static void DrawLines(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawLines(vertexArray);
		}

		inline static void SetViewPort(const uint32_t& width, const uint32_t height)
		{
			s_RendererAPI->SetViewport(width, height);
		}

		inline static void SetDepthMask(bool enabled)
		{
			s_RendererAPI->SetDepthMask(enabled);
		}

		inline static void SetCullFace(RendererAPI::CullFace face)
		{
			s_RendererAPI->SetCullFace(face);
		}

		inline static void SetStencilFunc(RendererAPI::StencilFunc func, int ref, uint32_t mask)
		{
			s_RendererAPI->SetStencilFunc(func, ref, mask);
		}

		inline static void SetStencilOp(RendererAPI::StencilOp sfail, RendererAPI::StencilOp dpfail, RendererAPI::StencilOp dppass)
		{
			s_RendererAPI->SetStencilOp(sfail, dpfail, dppass);
		}

		inline static void SetStencilMask(unsigned int mask)
		{
			s_RendererAPI->SetStencilMask(mask);
		}

		inline static void EnableStencilTest()
		{
			s_RendererAPI->EnableStencilTest(true);
		}

		inline static void DisableStencilTest()
		{
			s_RendererAPI->EnableStencilTest(false);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}
