// OpenGLContext.h
// OpenGL上下文类
// created by FirstRay2001, Nov/5/2025

#include "Ember/Renderer/GraphicsContext.h"

#pragma once

struct GLFWwindow;

namespace Ember
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;
	};
}

