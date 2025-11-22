// OpenGLRendererAPI.cpp
// OpenGL渲染API类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Ember
{
	void OpenGLRendererAPI::Init()
	{
		// 开启混合
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);

		// 开启面剔除
		glEnable(GL_CULL_FACE);
	}

	void* OpenGLRendererAPI::SetupMutiThread(void* window)
	{
		// 创建共享上下文窗口
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		void* sharedWindow =  glfwCreateWindow(1, 1, "", nullptr, (GLFWwindow*)window);
		if(sharedWindow == nullptr)
			EMBER_CORE_ERROR("Failed to create shared OpenGL context for multithreading!");
		return sharedWindow;
	}

	void OpenGLRendererAPI::InitMutiThread(void* window)
	{
		glfwMakeContextCurrent((GLFWwindow*)window);
	}

	void OpenGLRendererAPI::ShutdownMutiThread()
	{
		GLFWwindow* currentWindow = glfwGetCurrentContext();
		if (currentWindow)
		{
			glfwDestroyWindow(currentWindow);
		}
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		glDrawElements(GL_LINES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::SetViewport(const uint32_t& width, const uint32_t& height)
	{
		glViewport(0, 0, width, height);
	}
}