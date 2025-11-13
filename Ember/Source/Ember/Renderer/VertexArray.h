// VertexArray.h
// 顶点数组类
// created by FirstRay2001, Nov/6/2025

#pragma once
#include "Ember/Core/Core.h"
#include "Buffer.h"
#include <glm/glm.hpp>

namespace Ember
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void AddVertexBuffer(const Ref<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> indexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		static Ref<VertexArray> Create();
		static Ref<VertexArray> CreateCube(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
		static Ref<VertexArray> CreateSphere(float radius = 1.0f, uint32_t sectorCount = 36, uint32_t stackCount = 18);
	};
}


