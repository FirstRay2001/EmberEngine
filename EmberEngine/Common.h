// Common.h
// 通用头文件、通用宏定义
// created by FirstRay2001, Sep/15/2025

#pragma once

//////// 头文件 ////////
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>	// TODO: 自己写一个string？
#include <fstream>
#include <sstream>
#include <filesystem> // TODO: 自己的FileSystem

#include "Libraries/Foundation/Include/Math.h"
#include "Libraries/Foundation/Include/Memory.h"
#include "Libraries/Foundation/Include/Collection.h"


//////// 前置声明类 ////////
class FScene;
class FShader;
class FMesh;
class FTexture;

//////// 宏开关 ////////
#define DEBUG 1


//////// 宏定义 ////////
#ifdef  DEBUG
	#define LOG_INFO(fmt, ...)	printf("[INFO] " fmt "\n", ##__VA_ARGS__) 
	#define LOG_WARN(fmt, ...)	printf("[WARN] " fmt "\n", ##__VA_ARGS__)
	#define LOG_ERROR(fmt, ...)	printf("[ERROR] " fmt "\n", ##__VA_ARGS__)
	#define LOG_DEBUG(fmt, ...)	printf("[DEBUG][%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
	#define LOG_INFO(fmt, ...)
	#define LOG_WARN(fmt, ...)
	#define LOG_ERROR(fmt, ...)
	#define LOG_DEBUG(fmt, ...)
#endif //  DEBUG

#ifdef DEBUG
	#define ASSERT(condition, message)										\
	do{																		\
		if(!(condition))													\
		{																	\
			fprintf(stderr, "Assertion failed: %s, File: %s, Line: %d\n",	\
					message, __FILE__, __LINE__);							\
			abort();														\
		}																	\
	}																		\
	while(0)
#else
	#define ASSERT(condition, message)
#endif // DEBUG

#define IfFailGo(condition)				do{ if (!(condition)) return; } while(0)
#define IfFailRet(condition, RetValue)	do{ if (!(condition)) return RetValue;}while(0)





