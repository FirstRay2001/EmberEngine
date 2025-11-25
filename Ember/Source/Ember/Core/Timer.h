// Timer.h
// 计时器
// created by FirstRay2001, Nov/25/2025

#pragma once

#include <chrono>

namespace Ember
{
	// 计时器类
	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		float Elapsed() const
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<float>(endTimepoint - m_StartTimepoint).count();
		}

		float ElapsedMillis() const
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	};
}
