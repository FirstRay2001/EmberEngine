// Event.h
// 事件类
// created by FirstRay2001, Nov/01/2025

#pragma once

#include "emberpch.h"
#include "Ember/Core.h"

namespace Ember
{
	// 事件类型
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// 事件类别
	enum EventCategory
	{
		None = 0,
		EventCategotyApplication		= BIT(0),
		EventCategoryInput				= BIT(1),
		EventCategoryKeyboard			= BIT(2),
		EventCategoryMouse				= BIT(3),
		EventCategoryMouseButton		= BIT(4)
	};

	// 事件声明宏
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }					\
								virtual EventType GetEventType() const override { return GetStaticType(); }	\
								virtual const char* GetName() const override { return #type; }
	// 事件类别声明宏
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// 事件基类
	class EMBER_API Event
	{
		friend class EventDispatcher;
	public:
		// 获取事件类型
		virtual EventType GetEventType() const = 0;

		// 获取事件名称
		virtual const char* GetName() const = 0;

		// 获取事件类别标志
		virtual int GetCategoryFlags() const = 0;

		// 格式化字符串输出
		virtual std::string ToString() const { return GetName(); }

		// 是否属于某个类别
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	protected:
		// 已处理标记
		bool m_Handled = false; 
	};

	// 事件分发器
	class EventDispatcher
	{
		template<typename T>
		using Fn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// 分发事件
		template<typename T>
		bool Dispatch(Fn<T>& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	// 重载输出运算符
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
