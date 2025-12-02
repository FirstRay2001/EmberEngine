// ScriptGlue.h
// 脚本系统胶水代码
// created by FirstRay2001, Dec/1/2025

#include "emberpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "Ember/Core/Input.h"
#include "Ember/Core/KeyCode.h"
#include "Ember/Core/UUID.h"
#include "Ember/Scene/Scene.h"

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

	static bool Input_IsKeyPressed(KeyCode key)
	{
		return Input::IsKeyPressed(key);
	}

	static void Transform_GetPosition(UUID uuid, glm::vec3* outPosition)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outPosition = entity.GetComponent<TransformComponent>().Position;
	}

	static void Transform_SetPosition(UUID uuid, glm::vec3* inPosition)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Position = *inPosition;
	}

	void ScriptGlue::RegisterComponents()
	{
	}

	void ScriptGlue::RegisterFunctions()
	{
		EMBER_ADD_INTERNAL_CALL(NativeLog);
		EMBER_ADD_INTERNAL_CALL(Input_IsKeyPressed);
		EMBER_ADD_INTERNAL_CALL(Transform_GetPosition);
		EMBER_ADD_INTERNAL_CALL(Transform_SetPosition);
	}
}