// Application.h
// 应用类
// created by FirstRay2001, Oct/30/2025

#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace Ember
{
	class EMBER_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(class Event& e);

		bool OnWindowClose(class WindowCloseEvent& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true;
	};

	// 在客户端实现此函数以创建应用实例
	Application* CreateApplication();
}