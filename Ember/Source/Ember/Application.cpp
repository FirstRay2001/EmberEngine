// Application.cpp
// 应用类
// created by FirstRay2001, Oct/30/2025

#include "emberpch.h"
#include "Application.h"
#include "Ember/Events/ApplicationEvent.h"
#include "Ember/ImGui/ImGuiLayer.h"

// test
#include "Ember/Renderer/Renderer.h"

using namespace Ember;

Application* Application::s_Instance = nullptr;

Application::Application()
{
	EMBER_CORE_ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	m_Window = Scope<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);


	// Graphics Test
	m_VertexArray = Ref<VertexArray>(VertexArray::Create());
	float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};
	Ref<VertexBuffer> vertexBuffer(VertexBuffer::Create(vertices, sizeof(vertices)));
	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	Ref<IndexBuffer> indexBuffer(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_Shader = Ref<Shader>(Shader::Create(
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
}

void Application::Run()
{
	while (m_Running)
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// test
		Camera cam(float(GetWindow().GetWidth())/float(GetWindow().GetHeight()));
		cam.SetPosition({ 0.0f, 0.0f, 3.0f });
		cam.SetRotation(30.0f);
		Renderer::BeginScene(cam);
		{
			Renderer::Submit(m_Shader, m_VertexArray);
		}
		Renderer::EndScene();

		// 逻辑更新
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

		// ImGui更新
		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}
		m_ImGuiLayer->End();

		// 更新窗口
		m_Window->OnUpdate();
	}
}

void Ember::Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
	
	// 自顶向下遍历图层堆栈
	for (auto It = m_LayerStack.end(); It != m_LayerStack.begin(); )
	{
		(*--It)->OnEvent(e);
		if (e.Handled)
			break;
	}
}

bool Ember::Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

void Ember::Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
}

void Ember::Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PushOverlay(overlay);
}
