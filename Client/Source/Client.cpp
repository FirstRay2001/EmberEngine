// Client.cpp
// 游戏客户端入口
// created by FirstRay2001, Oct/30/2025

#include <Ember.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Ember::Layer
{
public:
	ExampleLayer() : 
		Ember::Layer("ExampleLayer"),
		m_Camera(Ember::CreateScope<Ember::Camera>(Ember::Camera(16.0f/9.0f)))
	{
		// Graphics Test
		m_VertexArray = Ember::Ref<Ember::VertexArray>(Ember::VertexArray::Create());
		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		Ember::Ref<Ember::VertexBuffer> vertexBuffer(Ember::VertexBuffer::Create(vertices, sizeof(vertices)));
		Ember::BufferLayout layout = {
			{ Ember::ShaderDataType::Float3, "a_Position" },
			{ Ember::ShaderDataType::Float2, "a_UV" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[] = { 
			0,1,2,
			0,2,4,
			6,7,8,
			6,8,10,
			12,13,14,
			12,14,16,
			18,19,20,
			18,20,22,
			24,25,26,
			24,26,28,
			30,31,32,
			30,32,34 };
		Ember::Ref<Ember::IndexBuffer> indexBuffer(Ember::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader = Ember::Ref<Ember::Shader>(Ember::Shader::Create(
			"#version 330 core\n"
			"layout(location = 0) in vec3 a_Position;\n"
			"layout(location = 1) in vec2 a_UV;\n"
			"out vec2 TexCoord;\n"
			"uniform mat4 u_ViewProjection;\n"
			"uniform mat4 u_Transform;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);\n"
			"	TexCoord = a_UV;\n"
			"}\n",
			"#version 330 core\n"
			"in vec2 TexCoord;"
			"out vec4 color;\n"
			"uniform sampler2D u_Texture;\n"
			"uniform vec3 u_Color;\n"
			"void main()\n"
			"{\n"
			"	vec3 sampleColor = texture(u_Texture, TexCoord).rgb;\n"
			"	color = vec4(sampleColor, 1.0);\n"
			"}\n"
		));

		m_Texture = Ember::Texture2D::Create("Asset/Texture/Checkerboard.png");

		OPENGLSHADER(m_Shader)->Bind();
		OPENGLSHADER(m_Shader)->SetUniformInt("u_Texture", 0);

		m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });
	}


	virtual void OnUpdate(const Ember::Timestep& timestep) override
	{
		float deltaSeconds = timestep.GetSeconds();
		float moveAmount = m_MoveSpeed * deltaSeconds;
		float rotateAmount = m_RotateSpeed * deltaSeconds;

		// 相机移动逻辑
		if (Ember::Input::IsKeyPressed(EMBER_KEY_A))
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f) * moveAmount);
		if (Ember::Input::IsKeyPressed(EMBER_KEY_D))
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f) * moveAmount);
		if (Ember::Input::IsKeyPressed(EMBER_KEY_W))
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f) * moveAmount);
		if (Ember::Input::IsKeyPressed(EMBER_KEY_S))
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f) * moveAmount);

		// 相机旋转逻辑
		if (Ember::Input::IsKeyPressed(EMBER_KEY_Q))
			m_Camera->SetRotation(m_Camera->GetRotation() + rotateAmount);
		if (Ember::Input::IsKeyPressed(EMBER_KEY_E))
			m_Camera->SetRotation(m_Camera->GetRotation() - rotateAmount);

		// 设置Shader颜色Uniform
		m_Shader->Bind();
		OPENGLSHADER(m_Shader)->SetUniformFloat3("u_Color", m_BoxColor);

		// 渲染场景
		Ember::Renderer::BeginScene(*m_Camera.get());
		{
			/*for (int y = 0; y < 20; y++)
			{
				for (int x = 0; x < 20; x++)
				{
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.11f, y * 0.11f, 0.0f)) *
						glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 1.0f));
					Ember::Renderer::Submit(m_Shader, m_VertexArray, transform);
				}
			}*/

			m_Texture->Bind();
			Ember::Renderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));

		}
		Ember::Renderer::EndScene();
	}

	virtual void OnEvent(Ember::Event& e) override
	{
		Ember::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Ember::WindowResizeEvent>([this](Ember::WindowResizeEvent& e)
			{
				Ember::RenderCommand::SetViewPort(e.GetWidth(), e.GetHeight());
				m_Camera->SetScreentSize(e.GetWidth(), e.GetHeight());
				return false;
			});
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_BoxColor));
		ImGui::End();
	}

private:
	Ember::Ref<Ember::VertexArray> m_VertexArray;
	Ember::Ref<Ember::Shader> m_Shader;
	Ember::Scope<Ember::Camera> m_Camera;
	Ember::Ref<Ember::Texture2D> m_Texture;
	glm::vec3 m_BoxColor = { 0.2f, 0.32f, 0.8f };

	float m_MoveSpeed = 1.0f;
	float m_RotateSpeed = 200.0f;
};

class GameClient : public Ember::Application
{
public:
	GameClient() 
	{
		PushLayer(new ExampleLayer());
	}

	~GameClient() {}
};

EMBER_IMPL_ENTRY_POINT(GameClient);