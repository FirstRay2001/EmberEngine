// ApplicationEvent.h
// 应用程序事件
// created by FirstRay2001, Nov/01/2025

#pragma once

#include "Event.h"

namespace Ember
{
	// 调整窗口大小事件
	class EMBER_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height) :
			m_Width(width),
			m_Height(height)
		{
		}

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategotyApplication)

	private:
		int m_Width, m_Height; // 新宽度和高度
	};

	// 关闭窗口事件
	class EMBER_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategotyApplication)
	};

	// Tick事件
	class EMBER_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategotyApplication)
	};

	// Update事件
	class EMBER_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategotyApplication)
	};

	// Render事件
	class EMBER_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategotyApplication)
	};

}

