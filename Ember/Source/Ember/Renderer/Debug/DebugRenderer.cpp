// DebugRenderer.cpp
// Debug绘制
// created by FirstRay2001, Dec/16/2025

#include "emberpch.h"
#include "DebugRenderer.h"
#include "Ember/Renderer/Shader.h"
#include "Ember/ResourceManager/ShaderLibrary.h"
#include "Ember/Renderer/Buffer.h"

namespace Ember
{
	DebugRenderer::DebugRenderer()
	{
		m_Shader = ShaderLibrary::Get().LoadSync("Asset/Shader/SimpleColor.glsl");
	}

	void DebugRenderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1)
	{
		m_Points.push_back(p0.x);
		m_Points.push_back(p0.y);
		m_Points.push_back(p0.z);
		m_Points.push_back(0);
		m_Points.push_back(0);
		m_Points.push_back(0);
		m_Points.push_back(0);
		m_Points.push_back(0);
		m_Points.push_back(p1.x);
		m_Points.push_back(p1.y);
		m_Points.push_back(p1.z);
		m_Points.push_back(0);
		m_Points.push_back(0);
		m_Points.push_back(0);
		m_Points.push_back(0);
		m_Points.push_back(0);
	}


	// TODO: 优化此函数，避免每次都创建新的VAO和VBO
	void DebugRenderer::FlushLines()
	{
		if (m_Points.empty())
			return;

		Ref<VertexArray> VAO = VertexArray::Create();
		Ref<VertexBuffer> VBO = VertexBuffer::Create(m_Points.data(), m_Points.size() * sizeof(float));

		std::vector<uint32_t> indices;
		for (uint32_t i = 0; i < m_Points.size() / 8; i++)
		{
			indices.push_back(i * 2);
			indices.push_back(i * 2 + 1);
		}
		Ref<IndexBuffer> IBO = IndexBuffer::Create(indices.data(), indices.size());

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" },
		};
		VBO->SetLayout(layout);
		VAO->AddVertexBuffer(VBO);
		VAO->SetIndexBuffer(IBO);
		
		Renderer::DrawLines(m_Shader, VAO, glm::mat4(1.0f));
		m_Points.clear();
	}
}