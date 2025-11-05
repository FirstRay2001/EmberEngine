// OpenGLContext.cpp
// OpenGL上下文类
// created by FirstRay2001, Nov/5/2025

#include "emberpch.h"
#include "OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Ember
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
		m_Window(static_cast<GLFWwindow*>(windowHandle))
	{
		EMBER_ASSERT(m_Window, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		// 设置窗口上下文
		glfwMakeContextCurrent(m_Window);

		// 初始化GLAD
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EMBER_CORE_ASSERT(status, "Failed to initialize Glad!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}