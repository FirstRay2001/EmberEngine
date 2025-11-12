// ImGuiLayer.h
// ImGui图层
// created by FirstRay2001, Nov/4/2025

#pragma once

#include "Ember/Core/Core.h"
#include "Ember/Core/Layer.h"

namespace Ember
{
	class EMBER_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = false;
	};
}