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

		// 创建顶点数组对象
        auto vertexArray = Create();
		vertexArray->m_PrimitiveType = PrimitiveType::Cube;
		vertexArray->m_CubeScale = scale;

		// 创建顶点缓冲区
        auto vertexBuffer(VertexBuffer::Create(vertices, sizeof(vertices)));

		// 设置缓冲区布局
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

    Ref<VertexArray> VertexArray::CreateSphere(float radius, uint32_t sectorCount, uint32_t stackCount)
    {
        std::vector<float> vertices;
        std::vector<uint32_t> indices;

        // 生成顶点数据
        for (uint32_t i = 0; i <= stackCount; ++i) {
            float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stackCount; // 从pi/2到-pi/2
            float xy = radius * cosf(stackAngle);             // r * cos(u)
            float z = radius * sinf(stackAngle);              // r * sin(u)

            for (uint32_t j = 0; j <= sectorCount; ++j) {
                float sectorAngle = j * 2 * glm::pi<float>() / sectorCount; // 从0到2pi
                float x = xy * cosf(sectorAngle);           // r * cos(u) * cos(v)
                float y = xy * sinf(sectorAngle);           // r * cos(u) * sin(v)

                // 位置
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // 法线
                glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);

                // 纹理坐标
                float s = (float)j / sectorCount;
                float t = (float)i / stackCount;
                vertices.push_back(s);
                vertices.push_back(t);
            }
        }

        // 生成索引数据
        for (uint32_t i = 0; i < stackCount; ++i) {
            uint32_t k1 = i * (sectorCount + 1);     // 当前栈的第一个顶点
            uint32_t k2 = k1 + sectorCount + 1;      // 下一个栈的第一个顶点

            for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    // 第一个三角形
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1)) {
                    // 第二个三角形
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        // 创建顶点数组对象
        auto vertexArray = VertexArray::Create();
		vertexArray->m_PrimitiveType = PrimitiveType::Sphere;
		vertexArray->m_SphereRadius = radius;
		vertexArray->m_SphereSectorCount = sectorCount;
		vertexArray->m_SphereStackCount = stackCount;

        // 创建顶点缓冲区
        auto vertexBuffer = VertexBuffer::Create(vertices.data(), (uint32_t)vertices.size() * sizeof(float));

        // 设置缓冲区布局
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };
        vertexBuffer->SetLayout(layout);
        vertexArray->AddVertexBuffer(vertexBuffer);

        // 创建索引缓冲区
        auto indexBuffer = IndexBuffer::Create(indices.data(), (uint32_t)indices.size());
        vertexArray->SetIndexBuffer(indexBuffer);

        return vertexArray;
    }
}