// WindowsPlatformUtils.cpp
// Windows平台工具实现
// created by FirstRay2001, Nov/23/2025

#include "emberpch.h"
#include "Ember/Utils/PlatformUtils.h"

#include <sstream>
#include <commdlg.h>
#include <shlobj.h>
#include <GLFW/glfw3.h>
#define  GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Ember/Core/Application.h"

namespace Ember
{
	std::string FileDialog::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;       // 结构体变量
		CHAR szFile[260] = { 0 };       // 文件名缓冲区
		ZeroMemory(&ofn, sizeof(ofn));	// 结构体清零
		ofn.lStructSize = sizeof(ofn); 		 // 结构体大小
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow()); // 所属窗口句柄
		ofn.lpstrFile = szFile; 			 // 文件名缓冲区
		ofn.nMaxFile = sizeof(szFile); 	 // 文件名缓冲区大小
		ofn.lpstrFilter = filter; 	 // 过滤器
		ofn.nFilterIndex = 1; 		 // 过滤器索引
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		// 显示打开文件对话框
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return std::string(ofn.lpstrFile);
		}

		// 用户取消或出错，返回空字符串
		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;       // 结构体变量
		CHAR szFile[260] = { 0 };       // 文件名缓冲区
		ZeroMemory(&ofn, sizeof(ofn));	// 结构体清零
		ofn.lStructSize = sizeof(ofn); 		 // 结构体大小
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow()); // 所属窗口句柄
		ofn.lpstrFile = szFile; 			 // 文件名缓冲区
		ofn.nMaxFile = sizeof(szFile); 	 // 文件名缓冲区大小
		ofn.lpstrFilter = filter; 	 // 过滤器
		ofn.nFilterIndex = 1; 		 // 过滤器索引

		ofn.lpstrDefExt = strchr(filter, '\0') + 1; // 默认扩展名，取过滤器中的第一个扩展名

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// 显示保存文件对话框
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return std::string(ofn.lpstrFile);
		}

		// 用户取消或出错，返回空字符串
		return std::string();
	}

	std::string FileDialog::OpenFolder()
	{	
		BROWSEINFOA bi;
		CHAR path[MAX_PATH];
		ZeroMemory(&bi, sizeof(bi));
		bi.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		bi.lpszTitle = "Select Folder";
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
		if (pidl != nullptr)
		{
			// 获取选中的路径
			if (SHGetPathFromIDListA(pidl, path))
			{
				CoTaskMemFree(pidl);
				return std::string(path);
			}
			CoTaskMemFree(pidl);
		}
		return std::string();
	}
}