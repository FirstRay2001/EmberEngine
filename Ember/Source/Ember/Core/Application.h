// Application.h
// 应用类
// created by FirstRay2001, Oct/30/2025

#pragma once

#include "Ember/Core/Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace Ember
{
	class EMBER_API Application
	{
	public:
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		
	public:
		Application();
		virtual ~Application() = default;
		void Run();
		void Close();

		void OnEvent(class Event& e);

		bool OnWindowClose(class WindowCloseEvent& e);
		bool OnWindowResize(class WindowResizeEvent& e);
		bool OnKeyPressed(class KeyPressedEvent& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		Scope<Window> m_Window;
		class ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	// 在客户端实现此函数以创建应用实例
	Application* CreateApplication();
}