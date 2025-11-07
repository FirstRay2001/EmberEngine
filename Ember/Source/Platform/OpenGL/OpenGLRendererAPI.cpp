// OpenGLRendererAPI.cpp
// OpenGL渲染API类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Ember
{
	void OpenGLRendererAPI::Init()
	{
		// 开启混合
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::SetViewport(const uint32_t& width, const uint32_t& height)
	{
		glViewport(0, 0, width, height);
	}
}