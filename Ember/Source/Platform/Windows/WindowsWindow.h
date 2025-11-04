// WindowsWindow.h
// Windows窗口
// created by FirstRay2001, Nov/2/2025

#pragma once

#include "Ember/Window.h"

struct GLFWwindow;

namespace Ember
{
	class WindowsWindow : public Ember::Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		// 设置事件回调函数
		void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}