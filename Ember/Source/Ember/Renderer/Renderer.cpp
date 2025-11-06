// Renderer.cpp
// 渲染器类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Renderer.h"

namespace Ember
{
	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}