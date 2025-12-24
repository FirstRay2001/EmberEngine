// ScriptEngine.h
// 脚本引擎接口
// created by FirstRay2001, Nov/30/2025

#include "emberpch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace Ember
{
	namespace Utils
	{
		// 读取文件字节
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream.is_open())
			{
				EMBER_CORE_ERROR("Failed to open file: {0}", filepath.string());
				*outSize = 0;
				return nullptr;
			}

			std::streamsize end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				EMBER_CORE_ERROR("File is empty: {0}", filepath.string());
				*outSize = 0;
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read(buffer, size);
			stream.close();
			*outSize = size;
			return buffer;
		}

		// 加载程序集
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& filepath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(filepath, &fileSize);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				EMBER_CORE_ERROR("Failed to open Mono image from data: {0}", filepath.string());
				delete[] fileData;
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, filepath.string().c_str(), &status, 0);

			mono_image_close(image);
			delete[] fileData;
			return assembly;
		}

		// 打印程序集中的类型
		static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int rows = mono_table_info_get_rows(typeDefTable);

			for (int i = 0; i < rows; ++i)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefTable, i, cols, MONO_TYPEDEF_SIZE);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
				const char* namespaze = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				EMBER_CORE_INFO("Type: {}.{}", namespaze, name);
			}
		}
	}

	// 脚本引擎上下文
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;
		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;
		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;

		// Scene上下文
		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		// 初始化Mono环境
		InitMono();

		// 加载核心程序集
		LoadAssembly("Resources/Scripts/Ember-ScriptCore.dll");

		// 加载APP程序集
		LoadAppAssembly("SandboxProject/Asset/Script/Binaries/Sandbox.dll");
				
		// 加载Entity类
		LoadAssemblyClasses();

		// 注册胶水代码
		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

#if 0
		// 加载MonoBehaviour类
		s_Data->MonoBehaviourClass = ScriptClass("Ember", "MonoBehaviour");

		//// TEST ////
		auto* instance = s_Data->MonoBehaviourClass.Instantiate();
		auto* method = s_Data->MonoBehaviourClass.GetMethod("OnStart", 0);
		s_Data->MonoBehaviourClass.InvokeMethod(instance, method);
#endif
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		// 设置场景上下文
		s_Data->SceneContext = scene;

		// 注册脚本实例
		auto view = s_Data->SceneContext->m_Registry.view<ScriptComponent>();
		for (auto entity : view)
		{
			Entity scriptEntity{ entity, s_Data->SceneContext };
			OnCreateEntity(scriptEntity);
		}
	}

	void ScriptEngine::OnRuntimeStop()
	{
		// 清理场景上下文
		s_Data->SceneContext = nullptr;

		// 清理实体实例
		s_Data->EntityInstances.clear();
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& scriptComp = entity.GetComponent<ScriptComponent>();

		// 检查脚本类是否存在
		if (!EntityClassExists(scriptComp.Name))
			return;

		// 创建脚本实例
		Ref<ScriptClass> scriptClass = s_Data->EntityClasses[scriptComp.Name];
		Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(scriptClass, entity);
		s_Data->EntityInstances[entity.GetUUID()] = instance;

		// 调用OnCreate方法
		instance->SetState(ScriptInstance::LifecycleState::Created);
		instance->InvokeOnCreate();
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, float deltaTime)
	{
		auto it = s_Data->EntityInstances.find(entity.GetUUID());
		if (it == s_Data->EntityInstances.end())
		{
			EMBER_CORE_ASSERT(false, "Script instance not found for entity!");
			return;
		}

		// 获取脚本实例
		Ref<ScriptInstance> instance = it->second;

		// 当第一次调用
		if (instance->GetState() == ScriptInstance::LifecycleState::Created)
		{
			instance->SetState(ScriptInstance::LifecycleState::Running);
			instance->InvokeOnStart();
		}

		// 调用OnUpdate方法
		instance->InvokeOnUpdate(deltaTime);
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		// 创建应用域
		MonoDomain* appDomain = mono_domain_create_appdomain((char*)"EmberAppDomain", nullptr);
		EMBER_CORE_ASSERT(appDomain, "Failed to create Mono App Domain");
		s_Data->AppDomain = appDomain;

		// 加载核心程序集
		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath.string());
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		// Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		// 加载应用程序集
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath.string());
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
		// Utils::PrintAssemblyTypes(s_Data->AppAssembly);
	}

	void ScriptEngine::InitMono()
	{
		// Mono路径
		mono_set_assemblies_path("mono/lib");

		// 创建根域
		MonoDomain* rootDomain = mono_jit_init("EmberScriptRuntime");
		EMBER_CORE_ASSERT(rootDomain, "Failed to initialize Mono JIT");
		s_Data->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		// 卸载应用域
		if (s_Data->AppDomain)
		{
			// mono_domain_unload(s_Data->AppDomain);
			s_Data->AppDomain = nullptr;
		}

		// 关闭Mono JIT
		if (s_Data->RootDomain)
		{
			// mono_jit_cleanup(s_Data->RootDomain);
			s_Data->RootDomain = nullptr;
		}
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* scriptClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, scriptClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		// 清理现有类
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int rows = mono_table_info_get_rows(typeDefTable);
		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Ember", "Entity");
		s_Data->EntityClass = ScriptClass("Ember", "Entity", true);

		// 遍历类型定义表
		for (int i = 0; i < rows; ++i)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefTable, i, cols, MONO_TYPEDEF_SIZE);

			// 获取类名和命名空间
			const char* name = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);

			// 获取MonoClass
			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, name);

			// 跳过Entity类本身
			if(monoClass == entityClass)
				continue;

			// 检查是否是Entity的子类
			if (monoClass && mono_class_is_subclass_of(monoClass, entityClass, false))
			{
				// 组合完整类名
				std::string fullClassName = std::string(nameSpace) + "." + std::string(name);

				// 处理无命名空间的类
				if(strlen(nameSpace) == 0)
					fullClassName = name;

				// 创建ScriptClass
				Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, name);

				// 存储类
				s_Data->EntityClasses[fullClassName] = scriptClass;
				EMBER_CORE_INFO("Loaded Script Class: {}", fullClassName);
			}
		}
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool IsCore) :
		m_ClassNamespace(classNamespace),
		m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(IsCore? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, m_ClassNamespace.c_str(), m_ClassName.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int paramCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), paramCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
	{
		m_ScriptClass = scriptClass;
		m_Instance = m_ScriptClass->Instantiate();

		// 获取构造函数
		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		UUID uuid = entity.GetUUID();
		void* param = &uuid;	// 通过UUID构造
		m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);

		// 获取生命周期方法
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate", 0);
		m_OnStartMethod = m_ScriptClass->GetMethod("OnStart", 0);
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float deltaTime)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &deltaTime;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}

	void ScriptInstance::InvokeOnStart()
	{
		if (m_OnStartMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnStartMethod);
	}
}