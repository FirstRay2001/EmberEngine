// ImGuiLayer.h
// ImGui图层
// created by FirstRay2001, Nov/4/2025

#pragma once

#include "Ember/Core.h"
#include "Ember/Layer.h"

namespace Ember
{
	class EMBER_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& e) override;

		bool OnMouseButtonPressedEvent(class MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(class MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(class MouseMovedEvent& e);
		bool OnMouseScrolledEvent(class MouseScrolledEvent& e);
		bool OnKeyPressedEvent(class KeyPressedEvent& e);
		bool OnKeyReleasedEvent(class KeyReleasedEvent& e);
		bool OnKeyTypedEvent(class KeyTypedEvent& e);
		bool OnWindowResizeEvent(class WindowResizeEvent& e);

	private:
		float m_Time = 0.0f;
	};
}