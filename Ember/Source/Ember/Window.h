// Window.h
// 窗口类
// created by FirstRay2001, Nov/2/2025

#pragma once

#include "emberpch.h"
#include "Core.h"

namespace Ember
{
	// 窗口属性结构体
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Ember Engine", unsigned int width = 1280, unsigned int height = 720) :
			Title(title),
			Width(width),
			Height(height)
		{
		}
	};

	// 窗口基类
	class EMBER_API Window
	{
	public:
		// 事件回调函数类型
		using EventCallbackFn = std::function<void(class Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// 设置事件回调函数
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

		// 获取平台窗口句柄
		virtual void* GetNativeWindow() const = 0;
	};
}