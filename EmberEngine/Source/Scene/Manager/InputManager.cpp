// InputManager.cpp
// 输入管理器
// created by FirstRay2001, Oct/10/13

#include "InputManager.h"
#include "GLFW/glfw3.h"

void MInputManager::Initialize(GLFWwindow* Window)
{
	LOG_INFO("MInputManager Initialized");

	Window_ = Window;

	// 设置回调
	glfwSetKeyCallback(Window, KeyCallback);
	glfwSetMouseButtonCallback(Window, MouseButtonCallback);
	glfwSetCursorPosCallback(Window, CursorPosCallback);
	glfwSetScrollCallback(Window, ScrollCallback);

	// 初始化键盘状态
	for (int Key = 0; Key <= GLFW_KEY_LAST; Key++)
	{
		KeyStates_[Key] = GLFW_RELEASE;
	}
}

void MInputManager::Tick()
{
	MouseOffsetX_ = 0.0;
	MouseOffsetY_ = 0.0;
	ScrollOffset_ = 0.0;
}

bool MInputManager::IsKeyPressed(int Key) const
{
	auto It = KeyStates_.Find(Key);

	if (It != nullptr)
	{
		return *It == GLFW_PRESS || *It == GLFW_REPEAT;
	}

	return false;
}

bool MInputManager::IsMouseButtonPressed(int Button) const
{
	auto It = MouseButtonStates_.Find(Button);

	if (It != nullptr)
	{
		return *It == GLFW_PRESS;
	}

	return false;
}

MyMath::FVector2 MInputManager::GetMousePosition() const
{
	return MyMath::FVector2(MouseX_, MouseY_);
}

MyMath::FVector2 MInputManager::GetMouseOffset() const
{
	return MyMath::FVector2(MouseOffsetX_, MouseOffsetY_);
}

double MInputManager::GetScrollOffset() const
{
	return ScrollOffset_;
}

void MInputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	MInputManager::GetInstance().KeyStates_[key] = action;
}

void MInputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	MInputManager::GetInstance().MouseButtonStates_[button] = action;
}

void MInputManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	MInputManager& Instance = MInputManager::GetInstance();

	Instance.MouseOffsetX_ = xpos - Instance.MouseX_;

	// 反转Y轴
	Instance.MouseOffsetY_ = Instance.MouseY_ - ypos;
	Instance.MouseX_ = xpos;
	Instance.MouseY_ = ypos;
}

void MInputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	MInputManager::GetInstance().ScrollOffset_ = yoffset;
}
