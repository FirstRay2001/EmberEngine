// Controller.h
// 控制器类
// created by FirstRay2001, Nov/9/2025

#pragma once

namespace Ember
{
	class Timestep;
	class Event;

	class Controller
	{
	public:
		Controller() = default;
		virtual ~Controller() = default;
		virtual void OnUpdate(const Timestep& timestep) = 0;
		virtual void OnEvent(Event& e) = 0;
	};
}
