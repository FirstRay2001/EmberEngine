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

// 位操作
#define BIT(x) (1 << x)
