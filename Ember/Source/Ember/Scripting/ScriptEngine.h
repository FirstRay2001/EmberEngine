// ScriptEngine.h
// 脚本引擎
// created by FirstRay2001, Nov/30/2025

#pragma once

#include <string>
#include <filesystem>

#include "Ember/Scene/Entity.h"
#include "Ember/Scene/Scene.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}

namespace Ember
{
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool IsCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int paramCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		// 生命周期状态
		enum class LifecycleState
		{
			None,
			Created,
			Running,
			Destroyed
		};

	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

	public:
		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
		void InvokeOnStart();

		LifecycleState GetState() const { return m_State; }
		void SetState(LifecycleState state) { m_State = state; }

	private:
		Ref<ScriptClass> m_ScriptClass;
		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnStartMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		LifecycleState m_State = LifecycleState::None;
	};


	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float deltaTime);

		static Scene* GetSceneContext();
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();

		static MonoImage* GetCoreAssemblyImage();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* scriptClass);
		static void LoadAssemblyClasses();

		friend ScriptClass;
		friend class ScriptGlue;
	};
}
