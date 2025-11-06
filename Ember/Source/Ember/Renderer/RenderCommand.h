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

		inline static void SetViewPort(const uint32_t& width, const uint32_t height)
		{
			s_RendererAPI->SetViewport(width, height);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}
