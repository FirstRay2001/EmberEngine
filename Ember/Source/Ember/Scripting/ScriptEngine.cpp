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

		ScriptClass MonoBehaviourClass;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
		LoadAssembly("Resources/Scripts/Ember-ScriptCore.dll");
		
		// 注册胶水代码
		ScriptGlue::RegisterFunctions();

		// 加载MonoBehaviour类
		s_Data->MonoBehaviourClass = ScriptClass("Ember", "MonoBehaviour");

		//// TEST ////
		auto* instance = s_Data->MonoBehaviourClass.Instantiate();
		auto* method = s_Data->MonoBehaviourClass.GetMethod("OnStart", 0);
		s_Data->MonoBehaviourClass.InvokeMethod(instance, method);
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
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

	void ScriptEngine::InitMono()
	{
		// 设置Mono路径
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

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className) :
		m_ClassNamespace(classNamespace),
		m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(s_Data->CoreAssemblyImage, m_ClassNamespace.c_str(), m_ClassName.c_str());
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
}