// Application.cpp
// 应用类
// created by FirstRay2001, Oct/30/2025

#include "Application.h"
#include "Log.h"
#include "Ember/Events/ApplicationEvent.h"

using namespace Ember;
Application::Application()
{
}

Application::~Application()
{
}

void Application::Run()
{
	WindowResizeEvent event(1280, 720);
	if (event.IsInCategory(EventCategotyApplication))
	{
		EMBER_CORE_INFO("WindowResizeEvent is in Application Category");
	}
	if (event.IsInCategory(EventCategoryInput))
	{
		EMBER_CORE_INFO("WindowResizeEvent is in Input Category");
	}

	while (true)
	{
		// 游戏主循环
		break;
	}
}
