// Application.h
// 应用类
// created by FirstRay2001, Oct/30/2025

#pragma once

#include "Core.h"

namespace Ember
{
	class EMBER_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// 在客户端实现此函数以创建应用实例
	Application* CreateApplication();
}