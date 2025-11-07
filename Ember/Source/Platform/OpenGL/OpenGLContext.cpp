// OpenGLContext.cpp
// OpenGL上下文类
// created by FirstRay2001, Nov/5/2025

#include "emberpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

namespace Ember
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
		m_Window(static_cast<GLFWwindow*>(windowHandle))
	{
		EMBER_CORE_ASSERT(m_Window, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		// 设置窗口上下文
		glfwMakeContextCurrent(m_Window);

		// 初始化GLAD
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EMBER_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		EMBER_CORE_INFO("*----------OpenGL Info-----------");
		EMBER_CORE_INFO("* Vendor	: {0}", (const char*)glGetString(GL_VENDOR));
		EMBER_CORE_INFO("* Renderer	: {0}", (const char*)glGetString(GL_RENDERER));
		EMBER_CORE_INFO("* Version	: {0}", (const char*)glGetString(GL_VERSION));
		EMBER_CORE_INFO("*--------------------------------");

		// 启动深度测试
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}