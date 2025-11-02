// Application.cpp
// 应用类
// created by FirstRay2001, Oct/30/2025

#include "emberpch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

using namespace Ember;
Application::Application()
{
	m_Window = std::unique_ptr<Window>(Window::Create());
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
