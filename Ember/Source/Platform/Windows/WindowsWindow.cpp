// WindowsWindow.cpp
// Windows窗口
// created by FirstRay2001, Nov/2/2025

#include "emberpch.h"
#include "WindowsWindow.h"

namespace Ember
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		// 拉取事件
		glfwPollEvents();

		// 交换缓冲区
		glfwSwapBuffers(m_Window);
	}

	unsigned int WindowsWindow::GetWidth() const
	{
		return m_Data.Width;
	}

	unsigned int WindowsWindow::GetHeight() const
	{
		return m_Data.Height;
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.EventCallback = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		EMBER_CORE_TRACE("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		// 初始化GLFW
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			EMBER_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}

		// 创建GLFW窗口
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		// 设置窗口上下文
		glfwMakeContextCurrent(m_Window);

		// 设置用户指针
		glfwSetWindowUserPointer(m_Window, &m_Data);

		// 设置VSync
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		// 销毁GLFW窗口
		glfwDestroyWindow(m_Window);
	}
}
