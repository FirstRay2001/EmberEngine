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
    // 浏览回调函数
    static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
    {
        if (uMsg == BFFM_INITIALIZED)
        {
            SendMessageA(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        }
        return 0;
    }

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
        // 获取当前工作目录（项目目录）
        char projectDir[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, projectDir);
        std::string projectDirStr = projectDir;

        BROWSEINFOA bi;
        CHAR selectedPath[MAX_PATH];
        ZeroMemory(&bi, sizeof(bi));
        bi.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        bi.lpszTitle = "Select Folder";
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;

        // 确保路径格式正确
        std::string initialPath = projectDirStr;
        // 如果路径不以反斜杠结尾，添加一个
        if (!initialPath.empty() && initialPath.back() != '\\') {
            initialPath += '\\';
        }

        bi.lParam = (LPARAM)initialPath.c_str();
        bi.lpfn = BrowseCallbackProc;

        LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
        if (pidl != nullptr)
        {
            if (SHGetPathFromIDListA(pidl, selectedPath))
            {
                std::string fullPath = selectedPath;
                CoTaskMemFree(pidl);

                // 转换为相对路径
                return MakeRelativePath(projectDirStr, fullPath);
            }
            CoTaskMemFree(pidl);
        }
        return std::string();
    }

    // 将完整路径转换为相对于项目目录的相对路径
    std::string FileDialog::MakeRelativePath(const std::string& basePath, const std::string& fullPath)
    {
        // 如果完整路径不以基础路径开头，返回完整路径
        if (fullPath.find(basePath) != 0) {
            return fullPath;
        }

        // 提取相对路径部分
        std::string relativePath = fullPath.substr(basePath.length());

        // 去除开头的反斜杠
        if (!relativePath.empty() && (relativePath[0] == '\\' || relativePath[0] == '/')) {
            relativePath = relativePath.substr(1);
        }

        return relativePath;
    }

    
}