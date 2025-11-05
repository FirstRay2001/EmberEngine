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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}