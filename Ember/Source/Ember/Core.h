// Core.h
// 核心头文件
// created by FirstRay2001, Oct/30/2025

#pragma once

// 平台检测
#ifdef EMBER_PLATFORM_WINDOWS
	// DLL导入、导出
	#ifdef EMEBR_BUILD_DLL
		#define EMBER_API __declspec(dllexport)
	#else
		#define EMBER_API __declspec(dllimport)
	#endif
#else
	#error Ember only supports Windows!
#endif

// 断言宏
#ifdef EMBER_DEBUG
	#define EMBER_ENABLE_ASSERTS
#endif

#ifdef EMBER_ENABLE_ASSERTS
	#define EMBER_ASSERT(x, ...) { if(!(x)) { EMBER_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EMBER_CORE_ASSERT(x, ...) { if(!(x)) { EMBER_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EMBER_ASSERT(x, ...)
	#define EMBER_CORE_ASSERT(x, ...)
#endif


// 位操作
#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

