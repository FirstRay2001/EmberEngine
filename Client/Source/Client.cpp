// Client.cpp
// 游戏客户端入口
// created by FirstRay2001, Oct/30/2025

#include <Ember.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp> 
#include "Platform/OpenGL/OpenGLShader.h"

#include <GLFW/glfw3.h>

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
			{ Ember::ShaderDataType::Float2, "a_TexCoord" }
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

		m_ShaderLibrary = Ember::CreateRef<Ember::ShaderLibrary>();
		auto shader = m_ShaderLibrary->Load("Asset/Shader/TestTextureShader.glsl");

		m_Texture = Ember::Texture2D::Create("Asset/Texture/Checkerboard.png");

		OPENGLSHADER(shader)->Bind();
		OPENGLSHADER(shader)->SetUniformInt("u_Texture", 0);

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
		glm::quat NewQuat = m_Camera->GetRotation();
		if (Ember::Input::IsKeyPressed(EMBER_KEY_Q))
		{
			glm::vec3 eulers = glm::eulerAngles(NewQuat);
			eulers.z += glm::radians(rotateAmount);
			NewQuat = glm::quat(eulers);
		}
		if (Ember::Input::IsKeyPressed(EMBER_KEY_E))
		{
			glm::vec3 eulers = glm::eulerAngles(NewQuat);
			eulers.z -= glm::radians(rotateAmount);
			NewQuat = glm::quat(eulers);
		}
		m_Camera->SetRotation(NewQuat);


		// 渲染场景
		Ember::Renderer::BeginScene(*m_Camera.get());
		{
			// 设置Shader颜色Uniform
			auto shader = m_ShaderLibrary->Get("TestTextureShader");
			auto texture = m_Texture;
			shader->Bind();
			OPENGLSHADER(shader)->SetUniformFloat3("u_Color", m_BoxColor);
			texture->Bind();
			Ember::Renderer::Submit(shader, m_VertexArray, glm::mat4(1.0f));
		}
		Ember::Renderer::EndScene();
	}

	virtual void OnEvent(Ember::Event& e) override
	{
		Ember::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Ember::WindowResizeEvent>([this](Ember::WindowResizeEvent& e)
		{
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
	Ember::Ref<Ember::ShaderLibrary> m_ShaderLibrary;
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