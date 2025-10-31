// Log.cpp
// ��־ϵͳ
// created by FirstRay2001, Oct/31/2025

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Ember::Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Ember::Log::s_ClientLogger;

void Ember::Log::Init()
{
	// ������־��ʽ����־����
	spdlog::set_pattern("%^[%T] %n: %v%$");

	// ��ʼ��CoreLogger
	s_CoreLogger = spdlog::stdout_color_mt("EMBER");
	s_CoreLogger->set_level(spdlog::level::trace);

	// ��ʼ��ClientLogger
	s_ClientLogger = spdlog::stdout_color_mt("APP");
	s_ClientLogger->set_level(spdlog::level::trace);
}
