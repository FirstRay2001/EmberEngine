// PlatformUtils.h
// 平台工具
// created by FirstRay2001, Nov/23/2025

#pragma once

#include <string>

namespace Ember
{
	class FileDialog
	{
	public:
		// 打开文件对话框，返回选中的文件路径
		static std::string OpenFile(const char* filter = "All Files\0*.*\0");

		// 保存文件对话框，返回保存的文件路径
		static std::string SaveFile(const char* filter = "All Files\0*.*\0");
	};
}
