// Timestep.h
// 帧时间
// created by FirstRay2001, Nov/6/2025

#pragma once

namespace Ember
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) :
			m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}
