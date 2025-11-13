// VertexArray.cpp
// 顶点数组类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Ember
{
	Ref<VertexArray> VertexArray::Create()
	{
		// 根据当前渲染API创建顶点数组
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EMBER_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		}

		EMBER_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexArray> VertexArray::CreateCube(glm::vec3 scale)
	{
        auto vertexArray = Create();

        float vertices[] = {
            // 前面 (Z负方向)
            // 右前下
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0, 0.0f,
            // 左前下
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  scale.x, 0.0f,
            // 右前上
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0, scale.y,
            // 右前上
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0, scale.y,
            // 左前下
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  scale.x, 0.0f,
            // 左前上
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  scale.x, scale.y,

            // 后面 (Z正方向)
            // 右后上
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  scale.x, scale.y,
            // 左后下
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
            // 右后下
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  scale.x, 0.0f,
            // 左后上
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, scale.y,
            // 左后下
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
            // 右后上
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  scale.x, scale.y,

            // 左面 (X负方向)
            // 左前下
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0,
            // 左后下
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  scale.z, 0,
            // 左后上
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  scale.z, scale.y,
            // 左前下
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0,
            // 左后上
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  scale.z, scale.y,
            // 左前上
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  scale.y,

            // 右面 (X正方向)
            // 右前下
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  scale.z, 0,
            // 右前上
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  scale.z, scale.y,
            // 右后上
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0, scale.y,
            // 右前下
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  scale.z, 0,
            // 右后上
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0, scale.y,
            // 右后下
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0, 0,

            // 底面 (Y负方向)
            // 右后下
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0, 0.0f,
            // 左前下
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  scale.x, scale.z,
            // 右前下
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0, scale.z,
            // 左后下
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  scale.x, 0.0f,
            // 左前下
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  scale.x, scale.z,
            // 右后下
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0, 0.0f,

            // 顶面 (Y正方向)
            // 右前上
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  scale.x,   scale.z,
            // 左前上
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,      scale.z,
            // 右后上
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  scale.x,   0.0f,
            // 右后上
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  scale.x,   0.0f,
            // 左前上
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,      scale.z,
            // 左后上
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,      0.0f
        };

        // 应用缩放到顶点位置
        for (int i = 0; i < sizeof(vertices) / sizeof(float); i += 8) {
            vertices[i] *= scale.x;     // X坐标
            vertices[i + 1] *= scale.y; // Y坐标  
            vertices[i + 2] *= scale.z; // Z坐标
        }

        auto vertexBuffer(VertexBuffer::Create(vertices, sizeof(vertices)));
        BufferLayout layout = {
            { Ember::ShaderDataType::Float3, "a_Position" },
            { Ember::ShaderDataType::Float3, "a_Normal" },
            { Ember::ShaderDataType::Float2, "a_TexCoord" }
        };
        vertexBuffer->SetLayout(layout);
        vertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[] = {
            0,1,2, 3,4,5,
            6,7,8, 9,10,11,
            12,13,14, 15,16,17,
            18,19,20, 21,22,23,
            24,25,26, 27,28,29,
            30,31,32, 33,34,35
        };
        auto indexBuffer(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        vertexArray->SetIndexBuffer(indexBuffer);

        return vertexArray;
	}
}