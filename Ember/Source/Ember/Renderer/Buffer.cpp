// Buffer.cpp
// GPUBuffer类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Ember
{
	//////// 顶点缓冲区 ////////
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		// 根据当前渲染API创建顶点缓冲区
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	//////// 索引缓冲区 ////////
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		// 根据当前渲染API创建索引缓冲区
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}