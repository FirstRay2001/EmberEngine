// Application.cpp
// 应用类
// created by FirstRay2001, Oct/30/2025

#include "emberpch.h"
#include "Ember/Core/Application.h"
#include "Ember/Events/ApplicationEvent.h"
#include "Ember/ImGui/ImGuiLayer.h"

#include "Ember/Renderer/Renderer.h"
#include "Ember/Core/Timestep.h"

#include <GLFW/glfw3.h>

using namespace Ember;

Application* Application::s_Instance = nullptr;

Application::Application()
{
	EMBER_CORE_ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	m_Window = Scope<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	// 初始化Renderer
	Renderer::Init();

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
}

void Application::Run()
{
	while (m_Running)
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.2f, 1.0f });
		RenderCommand::Clear();

		// 更新帧时间
		float time = (float)glfwGetTime(); // 获取当前时间
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		// 逻辑更新(各层提交渲染命令)
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate(timestep);

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
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
	
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

bool Ember::Application::OnWindowResize(WindowResizeEvent& e)
{
	Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
	return false;
}

void Ember::Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);

	// 同步窗口大小
	WindowResizeEvent e(m_Window->GetWidth(), m_Window->GetHeight());
	layer->OnEvent(e);
}

void Ember::Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PushOverlay(overlay);
}
