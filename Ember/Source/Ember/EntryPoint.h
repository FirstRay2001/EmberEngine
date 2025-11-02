// EntryPoint.h
// 入口点
// created by FirstRay2001, Oct/30/2025

#pragma once

#ifdef EMBER_PLATFORM_WINDOWS

// 在客户端实现此函数以创建应用实例
extern Ember::Application* Ember::CreateApplication();

int main(int argc, char** argv)
{
	// 初始化日志系统
	Ember::Log::Init();

	EMBER_CORE_TRACE("Ember Engine started!");

	// 创建应用实例
	Ember::Application* app = Ember::CreateApplication();

	// 运行应用
	app->Run();

	// 释放
	delete app;

	return 0;
}

#else
	#error Ember only supports Windows!
#endif