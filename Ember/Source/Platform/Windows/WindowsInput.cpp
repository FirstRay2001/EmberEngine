// WindowsInput.h
// Windows输入管理器
// created by FirstRay2001, Nov/4/2025

#include "emberpch.h"
#include "WindowsInput.h"
#include "Ember/Core/Application.h"
#include "GLFW/glfw3.h"
#include "Ember/Core/KeyCode.h"

Ember::Input* Ember::Input::s_Instance = new Ember::WindowsInput();;

bool Ember::WindowsInput::IsKeyPressedImpl(int keycode)
{
	auto* window = static_cast<GLFWwindow*>(Ember::Application::Get().GetWindow().GetNativeWindow());
	int state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Ember::WindowsInput::IsMouseButtonPressedImpl(int button)
{
	auto* window = static_cast<GLFWwindow*>(Ember::Application::Get().GetWindow().GetNativeWindow());
	int state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Ember::WindowsInput::GetMousePositionImpl()
{
	auto* window = static_cast<GLFWwindow*>(Ember::Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}

float Ember::WindowsInput::GetMouseXImpl()
{
	auto [x, y] = GetMousePositionImpl();
	return x;
}

float Ember::WindowsInput::GetMouseYImpl()
{
	auto [x, y] = GetMousePositionImpl();
	return y;
}
