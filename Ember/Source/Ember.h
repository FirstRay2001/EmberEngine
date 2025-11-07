// Ember.h
// 客户端用引擎头
// created by FirstRay2001, Oct/30/2025

#pragma once

#include "Ember/Core.h"

#include "Ember/Application.h"
#include "Ember/Log.h"

#include "Ember/ImGui/ImGuiLayer.h"
#include "imgui/imgui.h"

// ----输入----------------------------------------------
#include "Ember/Input.h"
#include "Ember/KeyCode.h"
#include "Ember/Events/KeyEvent.h"
#include "Ember/Events/MouseEvent.h"
#include "Ember/Events/ApplicationEvent.h"
// ------------------------------------------------------

//-----渲染----------------------------------------------
#include "Ember/Renderer/Renderer.h"
#include "Ember/Renderer/RenderCommand.h"
#include "Ember/Renderer/VertexArray.h"
#include "Ember/Renderer/Shader.h"
#include "Ember/Renderer/Buffer.h"
#include "Ember/Renderer/Texture.h"
#include "Ember/Renderer/Camera.h"
//-------------------------------------------------------

// -------------入口点------------------------------------
#include "Ember/EntryPoint.h"
#define EMBER_IMPL_ENTRY_POINT(ApplicationClass)		\
	Ember::Application* Ember::CreateApplication()		\
	{													\
		return new ApplicationClass();					\
	}													\
// -------------------------------------------------------