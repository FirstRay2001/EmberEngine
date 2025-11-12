// RenderCommand.cpp
// 渲染命令
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ember
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}