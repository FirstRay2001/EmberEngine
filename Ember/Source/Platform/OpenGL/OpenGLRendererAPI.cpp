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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

	void OpenGLRendererAPI::SetDepthMask(bool enabled)
	{	
		if (enabled)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);
	}

	void OpenGLRendererAPI::SetCullFace(CullFace face)
	{
		switch (face)
		{
		case CullFace::None:
			glDisable(GL_CULL_FACE);
			break;
		case CullFace::Back:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case CullFace::Front:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		}
	}


	static GLenum ConvertStencilFunc(RendererAPI::StencilFunc func)
	{
		switch (func)
		{
		case Ember::RendererAPI::StencilFunc::NEVER:
			return GL_NEVER;
			break;
		case Ember::RendererAPI::StencilFunc::ALWAYS:
			return GL_ALWAYS;
			break;
		case Ember::RendererAPI::StencilFunc::LESS:
			return GL_LESS;
			break;
		case Ember::RendererAPI::StencilFunc::LEQUAL:
			return GL_LEQUAL;
			break;
		case Ember::RendererAPI::StencilFunc::GREATER:
			return GL_GREATER;
			break;
		case Ember::RendererAPI::StencilFunc::GEQUAL:
			return GL_GEQUAL;
			break;
		case Ember::RendererAPI::StencilFunc::EQUAL:
			return GL_EQUAL;
			break;
		case Ember::RendererAPI::StencilFunc::NOTEQUAL:
			return GL_NOTEQUAL;
			break;
		default:
			break;
		};
	}

	static GLenum ConvertStencilOp(RendererAPI::StencilOp op)
	{
		switch (op)
		{
		case Ember::RendererAPI::StencilOp::KEEP:
			return GL_KEEP;
			break;
		case Ember::RendererAPI::StencilOp::ZERO:
			return GL_ZERO;
			break;
		case Ember::RendererAPI::StencilOp::REPLACE:
			return GL_REPLACE;
			break;
		case Ember::RendererAPI::StencilOp::INCR:
			return GL_INCR;
			break;
		case Ember::RendererAPI::StencilOp::INCR_WRAP:
			return GL_INCR_WRAP;
			break;
		case Ember::RendererAPI::StencilOp::DECR:
			return GL_DECR;
			break;
		case Ember::RendererAPI::StencilOp::DECR_WRAP:
			return GL_DECR_WRAP;
			break;
		case Ember::RendererAPI::StencilOp::INVERT:
			return GL_INVERT;
			break;
		default:
			break;
		};
	}

	void OpenGLRendererAPI::EnableStencilTest(bool enabled)
	{
		if (enabled)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
	}

	void OpenGLRendererAPI::SetStencilFunc(RendererAPI::StencilFunc func, int ref, uint32_t mask)
	{
		glStencilFunc(ConvertStencilFunc(func), ref, mask);
	}

	void OpenGLRendererAPI::SetStencilOp(RendererAPI::StencilOp sfail, RendererAPI::StencilOp dpfail, RendererAPI::StencilOp dppass)
	{
		glStencilOp(ConvertStencilOp(sfail), ConvertStencilOp(dpfail), ConvertStencilOp(dppass));
	}

	void OpenGLRendererAPI::SetStencilMask(unsigned int mask)
	{
		glStencilMask(mask);
	}
}