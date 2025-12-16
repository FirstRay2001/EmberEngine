// ScriptGlue.h
// 脚本系统胶水代码
// created by FirstRay2001, Dec/1/2025

#include "emberpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "Ember/Core/Input.h"
#include "Ember/Core/KeyCode.h"
#include "Ember/Core/UUID.h"
#include "Ember/Core/Application.h"
#include "Ember/Scene/Scene.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

namespace Ember
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_HasComponentFuncs;

#define EMBER_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ember.InternalCalls::" #Name, Name);

	static void NativeLog(MonoString* message)
	{
		char* msg = mono_string_to_utf8(message);
		EMBER_CORE_INFO("[C#]: {0}", msg);
		mono_free(msg);
	}

	static void Window_CaptureMouse(bool capture)
	{
		Application::Get().GetWindow().SetMouseCapture(capture);
	}

	static bool Input_IsKeyPressed(KeyCode key)
	{
		return Input::IsKeyPressed(key);
	}

	static bool Input_IsMouseButtonPressed(int button)
	{
		return Input::IsMouseButtonPressed(button);
	}

	static void Input_GetMousePosition(glm::vec2* outPosition)
	{
		auto [x, y] = Input::GetMousePosition();
		outPosition->x = x;
		outPosition->y = y;
	}

	static bool Entity_HasComponent(UUID uuid, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		MonoType* monoType = mono_reflection_type_get_type(componentType);

		// 查找对应的HasComponent函数
		auto it = s_HasComponentFuncs.find(monoType);
		if (it == s_HasComponentFuncs.end())
		{
			EMBER_CORE_ERROR("No HasComponent function registered for the given MonoType!");
			return false;
		}
		return it->second(entity);
	}

	static void TransformComponent_GetPosition(UUID uuid, glm::vec3* outPosition)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outPosition = entity.GetComponent<TransformComponent>().Position;
	}

	static void TransformComponent_SetPosition(UUID uuid, glm::vec3* inPosition)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Position = *inPosition;
	}

	static void TransformComponent_GetRotation(UUID uuid, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID uuid, glm::vec3* inRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Rotation = *inRotation;
	}

	static void TransformComponent_GetScale(UUID uuid, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(UUID uuid, glm::vec3* inScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Scale = *inScale;
	}

	static void TransformComponent_GetForward(UUID uuid, glm::vec3* outForward)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outForward = entity.GetComponent<TransformComponent>().GetForward();
	}

	static void TransformComponent_GetRight(UUID uuid, glm::vec3* outRight)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outRight = entity.GetComponent<TransformComponent>().GetRight();
	}

	template<typename TComponent>
	static void RegisterComponent()
	{
		std::string_view typeName = typeid(TComponent).name();
		size_t pos = typeName.find_last_of(":");
		std::string_view structName = typeName.substr(pos + 1);
		std::string fullName = fmt::format("Ember.{}", structName);
		MonoType* monoType = mono_reflection_type_from_name((char*)fullName.c_str(), ScriptEngine::GetCoreAssemblyImage());
		if (!monoType)
		{
			EMBER_CORE_ERROR("Failed to find MonoType for component: {}", fullName);
			return;
		}

		s_HasComponentFuncs[monoType] = [](Entity entity)
		{
			return entity.HasComponent<TComponent>();
		};
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent<TransformComponent>();
		RegisterComponent<CameraComponent>();
		// RegisterComponent<NativeScriptComponent>();
		// RegisterComponent<DirectionalLightComponent>();
		// RegisterComponent<SkyboxComponent>();
		// RegisterComponent<ScriptComponent>();
	}

	void ScriptGlue::RegisterFunctions()
	{
		EMBER_ADD_INTERNAL_CALL(NativeLog);
		EMBER_ADD_INTERNAL_CALL(Window_CaptureMouse);
		EMBER_ADD_INTERNAL_CALL(Input_IsKeyPressed);
		EMBER_ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);
		EMBER_ADD_INTERNAL_CALL(Input_GetMousePosition);
		EMBER_ADD_INTERNAL_CALL(Entity_HasComponent);
		EMBER_ADD_INTERNAL_CALL(TransformComponent_GetPosition);
		EMBER_ADD_INTERNAL_CALL(TransformComponent_SetPosition);
		EMBER_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		EMBER_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		EMBER_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		EMBER_ADD_INTERNAL_CALL(TransformComponent_SetScale);
		EMBER_ADD_INTERNAL_CALL(TransformComponent_GetForward);
		EMBER_ADD_INTERNAL_CALL(TransformComponent_GetRight);
	}
}