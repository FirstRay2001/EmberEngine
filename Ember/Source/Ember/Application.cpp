// Application.cpp
// 应用类
// created by FirstRay2001, Oct/30/2025

#include "emberpch.h"
#include "Application.h"
#include "Ember/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

using namespace Ember;
Application::Application()
{
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
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

		// 更新窗口
		m_Window->OnUpdate();
	}
}

void Ember::Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	// EMBER_CORE_TRACE("{0}", e);
}

bool Ember::Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}
