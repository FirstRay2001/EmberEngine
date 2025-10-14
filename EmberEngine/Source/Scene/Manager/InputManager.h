// InputManager.h
// 输入管理器
// created by FirstRay2001, Oct/10/13

#pragma once

#include "Common.h"

class MInputManager
{
public:
	// 单例
	static MInputManager& GetInstance()
	{
		static MInputManager Instance;
		return Instance;
	}

public:
	// 初始化
	void Initialize(class GLFWwindow* Window);

	// 更新状态
	void Tick();

public:
	// 键盘按键是否处于按下状态
	bool IsKeyPressed(int Key) const;

	// 鼠标按键是否处于按下状态
	bool IsMouseButtonPressed(int Button) const;

	// 获取鼠标位置
	MyMath::FVector2 GetMousePosition() const;

	// 获取鼠标偏移量(相较于前一帧)
	MyMath::FVector2 GetMouseOffset() const;

	// 获取滚轮偏移量（相较于前一帧）
	double GetScrollOffset() const;

private:
	MInputManager() = default;

	// 键盘回调
	static void KeyCallback(class GLFWwindow* window, int key, int scancode, int action, int mods);

	// 鼠标按键回调
	static void MouseButtonCallback(class GLFWwindow* window, int button, int action, int mods);

	// 鼠标位置回调
	static void CursorPosCallback(class GLFWwindow* window, double xpos, double ypos);

	// 鼠标滚轮回调
	static void ScrollCallback(class GLFWwindow* window, double xoffset, double yoffset);

private:
	// glfw窗口
	class GLFWwindow* Window_ = nullptr;

	// 键盘状态
	MySTL::THashTable<int, int> KeyStates_;

	// 鼠标状态
	MySTL::THashTable<int, int> MouseButtonStates_;
	double MouseX_ = 0.0;
	double MouseY_ = 0.0;
	double MouseOffsetX_ = 0.0;
	double MouseOffsetY_ = 0.0;
	double ScrollOffset_ = 0.0;
};
