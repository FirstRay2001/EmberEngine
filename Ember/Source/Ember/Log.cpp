// Log.cpp
// 日志系统
// created by FirstRay2001, Oct/31/2025

#include "emberpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Ember::Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Ember::Log::s_ClientLogger;

void Ember::Log::Init()
{
	// 设置日志格式和日志级别
	spdlog::set_pattern("%^[%T] %n: %v%$");

	// 初始化CoreLogger
	s_CoreLogger = spdlog::stdout_color_mt("EMBER");
	s_CoreLogger->set_level(spdlog::level::trace);

	// 初始化ClientLogger
	s_ClientLogger = spdlog::stdout_color_mt("APP");
	s_ClientLogger->set_level(spdlog::level::trace);
}
