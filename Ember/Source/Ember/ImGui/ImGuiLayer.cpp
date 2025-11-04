// ImGuiLayer.cpp
// ImGui图层
// created by FirstRay2001, Nov/4/2025

#include "emberpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "GLFW/glfw3.h"
#include "Platform/OpenGL/OpenGLRenderer.h"

#include "Ember/Application.h"


namespace Ember
{
	ImGuiLayer::ImGuiLayer()
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendRendererName = "Ember_OpenGL_Renderer";
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init();
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		Application& app = Application::Get();
		ImGuiIO& io = ImGui::GetIO();

		// 设置显示尺寸
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// 更新帧时间
		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		// 渲染
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
	}
}