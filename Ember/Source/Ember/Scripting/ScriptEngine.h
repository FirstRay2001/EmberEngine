// ScriptEngine.h
// 脚本引擎接口
// created by FirstRay2001, Nov/30/2025

#pragma once

namespace Ember
{
	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();
	private:
		static void InitMono();
		static void ShutdownMono();
	};
}
