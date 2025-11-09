// Ember.h
// 客户端用引擎头
// created by FirstRay2001, Oct/30/2025

#pragma once

#include "Ember/Core/Core.h"

#include "Ember/Core/Application.h"
#include "Ember/Core/Log.h"

#include "Ember/ImGui/ImGuiLayer.h"
#include "imgui/imgui.h"

// ----输入----------------------------------------------
#include "Ember/Core/Input.h"
#include "Ember/Core/KeyCode.h"
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
#include "Ember/Renderer/Material.h"
#include "Ember/Renderer/Camera.h"
//-------------------------------------------------------