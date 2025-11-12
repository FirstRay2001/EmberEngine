// MouseEvent.h
// 鼠标事件
// created by FirstRay2001, Nov/01/2025

#pragma once

#include "Event.h"

namespace Ember
{
	// 鼠标移动事件
	class EMBER_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) :
			m_MouseX(x),
			m_MouseY(y)
		{
		}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX, m_MouseY; // 鼠标位置
	};

	// 鼠标滚动事件
	class EMBER_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) :
			m_XOffset(xOffset),
			m_YOffset(yOffset)
		{
		}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset; // 滚动偏移量
	};

	// 鼠标按钮事件基类
	class EMBER_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(int button) :
			m_Button(button)
		{
		}

		int m_Button;
	};

	// 鼠标按钮按下事件
	class EMBER_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) :
			MouseButtonEvent(button)
		{
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	// 鼠标按钮释放事件
	class EMBER_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) :
			MouseButtonEvent(button)
		{
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
