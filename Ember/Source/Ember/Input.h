// Input.h
// 输入管理器
// created by FirstRay2001, Nov/4/2025

#pragma once

#include "Ember/Core.h"

namespace Ember
{
	class EMBER_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode)
		{
			return s_Instance->IsKeyPressedImpl(keycode);
		}

		inline static bool IsMouseButtonPressed(int button)
		{
			return s_Instance->IsMouseButtonPressedImpl(button);
		}

		inline static std::pair<float, float> GetMousePosition()
		{
			return s_Instance->GetMousePositionImpl();
		}

		inline static float GetMouseX()
		{
			return s_Instance->GetMouseXImpl();
		}

		inline static float GetMouseY()
		{
			return s_Instance->GetMouseYImpl();
		}

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_Instance;
	};
}