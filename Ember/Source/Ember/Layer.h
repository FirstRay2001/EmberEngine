// Layer.cpp
// 图层
// created by FirstRay2001, Nov/2/2025

#pragma once

#include "Ember/Core.h"
#include "Ember/Events/Event.h"
#include "Ember/Core/Timestep.h"

namespace Ember
{
	class EMBER_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const Timestep& timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

	protected:
		std::string m_DebugName;
	};
}



