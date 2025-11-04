// WindowsInput.h
// Windows输入管理器
// created by FirstRay2001, Nov/4/2025

#pragma once

#include "Ember/Input.h"


namespace Ember
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}