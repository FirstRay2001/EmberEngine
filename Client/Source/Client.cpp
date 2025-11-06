// Client.cpp
// 游戏客户端入口
// created by FirstRay2001, Oct/30/2025

#include <Ember.h>

class ExampleLayer : public Ember::Layer
{
public:
	ExampleLayer() : 
		Ember::Layer("ExampleLayer"),
		m_Camera(Ember::CreateScope<Ember::Camera>(Ember::Camera(16.0f/9.0f)))
	{
		// Graphics Test
		m_VertexArray = Ember::Ref<Ember::VertexArray>(Ember::VertexArray::Create());
		float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Ember::Ref<Ember::VertexBuffer> vertexBuffer(Ember::VertexBuffer::Create(vertices, sizeof(vertices)));
		Ember::BufferLayout layout = {
			{ Ember::ShaderDataType::Float3, "a_Position" },
			{ Ember::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Ember::Ref<Ember::IndexBuffer> indexBuffer(Ember::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader = Ember::Ref<Ember::Shader>(Ember::Shader::Create(
			"#version 330 core\n"
			"layout(location = 0) in vec3 a_Position;\n"
			"layout(location = 1) in vec4 a_Color;\n"
			"out vec4 v_Color;\n"
			"uniform mat4 u_ViewProjection;\n"
			"void main()\n"
			"{\n"
			"	v_Color = a_Color;\n"
			"	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);\n"
			"}\n",
			"#version 330 core\n"
			"in vec4 v_Color;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"	color = v_Color;\n"
			"}\n"
		));

		m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });
	}


	virtual void OnUpdate() override
	{
		// 相机移动逻辑
		if (Ember::Input::IsKeyPressed(EMBER_KEY_A))
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(-0.05f, 0.0f, 0.0f));
		if (Ember::Input::IsKeyPressed(EMBER_KEY_D))
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.05f, 0.0f, 0.0f));
		if (Ember::Input::IsKeyPressed(EMBER_KEY_W))
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.0f, 0.05f, 0.0f));
		if (Ember::Input::IsKeyPressed(EMBER_KEY_S))
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.0f, -0.05f, 0.0f));

		// 相机旋转逻辑
		if (Ember::Input::IsKeyPressed(EMBER_KEY_Q))
			m_Camera->SetRotation(m_Camera->GetRotation() + 1.0f);
		if (Ember::Input::IsKeyPressed(EMBER_KEY_E))
			m_Camera->SetRotation(m_Camera->GetRotation() - 1.0f);

		// 渲染场景
		Ember::Renderer::BeginScene(*m_Camera.get());
		{
			Ember::Renderer::Submit(m_Shader, m_VertexArray);
		}
		Ember::Renderer::EndScene();
	}

	virtual void OnEvent(Ember::Event& e) override
	{
		Ember::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Ember::WindowResizeEvent>([this](Ember::WindowResizeEvent& e)
			{
				m_Camera->SetAspectRatio((float)e.GetWidth() / (float)e.GetHeight());
				return false;
			});
	}

	virtual void OnImGuiRender() override
	{
	}
private:
	Ember::Ref<Ember::VertexArray> m_VertexArray;
	Ember::Ref<Ember::Shader> m_Shader;
	Ember::Scope<Ember::Camera> m_Camera;
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