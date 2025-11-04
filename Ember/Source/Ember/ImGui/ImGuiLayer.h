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

	private:
		float m_Time = 0.0f;
	};
}