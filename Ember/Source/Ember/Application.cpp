// Application.cpp
// 应用类
// created by FirstRay2001, Oct/30/2025

#include "emberpch.h"
#include "Application.h"
#include "Ember/Events/ApplicationEvent.h"

#include <glad/glad.h>

using namespace Ember;

Application* Application::s_Instance = nullptr;

Application::Application()
{
	EMBER_CORE_ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application()
{
}

void Application::Run()
{
	while (m_Running)
	{
		// 设置清屏颜色
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// 清除屏幕
		glClear(GL_COLOR_BUFFER_BIT);

		// 游戏主循环
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

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
