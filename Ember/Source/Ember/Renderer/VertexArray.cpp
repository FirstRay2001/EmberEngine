// VertexArray.cpp
// 顶点数组类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Ember
{
	VertexArray* VertexArray::Create()
	{
		// 根据当前渲染API创建顶点数组
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}