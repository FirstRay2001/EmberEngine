// Core.h
// 核心头文件
// created by FirstRay2001, Oct/30/2025

#pragma once

// 平台检测
#ifdef EMBER_PLATFORM_WINDOWS
#ifdef EMBER_BUILD_DYNAMIC_LINK
	// DLL导入、导出
	#ifdef EMEBR_BUILD_DLL
		#define EMBER_API __declspec(dllexport)
	#else
		#define EMBER_API __declspec(dllimport)
	#endif
#else
	#define EMBER_API
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

// 智能指针封装
#include <memory>
namespace Ember
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

