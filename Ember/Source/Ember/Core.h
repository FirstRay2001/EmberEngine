// Core.h
// ����ͷ�ļ�
// created by FirstRay2001, Oct/30/2025

#pragma once

// ƽ̨���
#ifdef EMBER_PLATFORM_WINDOWS
	// DLL���롢����
	#ifdef EMEBR_BUILD_DLL
		#define EMBER_API __declspec(dllexport)
	#else
		#define EMBER_API __declspec(dllimport)
	#endif
#else
	#error Ember only supports Windows!
#endif
