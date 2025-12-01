// ScriptGlue.h
// 脚本系统胶水代码
// created by FirstRay2001, Dec/1/2025

#include "emberpch.h"
#include "ScriptGlue.h"

#include "mono/metadata/object.h"


namespace Ember
{
#define EMBER_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ember.InternalCalls::" #Name, Name);

	static void NativeLog(MonoString* message)
	{
		char* msg = mono_string_to_utf8(message);
		EMBER_CORE_INFO("[C#]: {0}", msg);
		mono_free(msg);
	}

	void ScriptGlue::RegisterComponents()
	{
	}

	void ScriptGlue::RegisterFunctions()
	{
		EMBER_ADD_INTERNAL_CALL(NativeLog);
	}
}