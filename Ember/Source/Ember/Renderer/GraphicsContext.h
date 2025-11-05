// GraphicsContext.h
// 图形上下文类
// created by FirstRay2001, Nov/5/2025

#pragma once

#include "Ember/Core.h"

namespace Ember
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}
