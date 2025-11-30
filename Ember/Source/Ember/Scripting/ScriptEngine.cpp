// ScriptEngine.h
// 脚本引擎接口
// created by FirstRay2001, Nov/30/2025

#include "emberpch.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace Ember
{
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;
		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	// 打印程序集中的类型
	void PrintAssemblyTypes(MonoAssembly* assembly)
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

	// 读取文件字节
	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream.is_open())
		{
			EMBER_CORE_ERROR("Failed to open file: {}", filepath);
			*outSize = 0;
			return nullptr;
		}

		std::streamsize end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			EMBER_CORE_ERROR("File is empty: {}", filepath);
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
	MonoAssembly* LoadAssembly(const std::string& filepath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(filepath, &fileSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			EMBER_CORE_ERROR("Failed to open Mono image from data: {}", filepath);
			delete[] fileData;
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, filepath.c_str(), &status, 0);
		
		mono_image_close(image);
		delete[] fileData;
		return assembly;
	}

	void ScriptEngine::InitMono()
	{
		// 设置Mono路径
		mono_set_assemblies_path("mono/lib");

		// 创建根域
		MonoDomain* rootDomain = mono_jit_init("EmberScriptRuntime");
		EMBER_CORE_ASSERT(rootDomain, "Failed to initialize Mono JIT");
		s_Data->RootDomain = rootDomain;

		// 创建应用域
		MonoDomain* appDomain = mono_domain_create_appdomain((char*)"EmberAppDomain", nullptr);
		EMBER_CORE_ASSERT(appDomain, "Failed to create Mono App Domain");
		s_Data->AppDomain = appDomain;

		// 加载核心程序集
		s_Data->CoreAssembly = LoadAssembly("Resources/Scripts/Ember-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);

		// TEST
		MonoImage* coreImage = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* testClass = mono_class_from_name(coreImage, "Ember", "Main");

		// 创建类实例
		MonoObject* instance = mono_object_new(s_Data->AppDomain, testClass);
		mono_runtime_object_init(instance);

		// 获取方法
		MonoMethod* printMessageFunc = mono_class_get_method_from_name(testClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

		// 获取参数方法
		MonoMethod* printIntFunc = mono_class_get_method_from_name(testClass, "PrintInt", 1);
		void* args[1];
		int32_t value = 42;
		args[0] = &value;
		mono_runtime_invoke(printIntFunc, instance, args, nullptr);
	}

	void ScriptEngine::ShutdownMono()
	{
		// 卸载应用域
		if (s_Data->AppDomain)
		{
			mono_domain_unload(s_Data->AppDomain);
			s_Data->AppDomain = nullptr;
		}

		// 关闭Mono JIT
		if (s_Data->RootDomain)
		{
			mono_jit_cleanup(s_Data->RootDomain);
			s_Data->RootDomain = nullptr;
		}
	}
}