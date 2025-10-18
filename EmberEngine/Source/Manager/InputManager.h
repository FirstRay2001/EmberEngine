// InputManager.h
// ���������
// created by FirstRay2001, Oct/10/13

#pragma once

#include "Common.h"

class MInputManager
{
public:
	// ����
	static MInputManager& GetInstance()
	{
		static MInputManager Instance;
		return Instance;
	}

public:
	// ��ʼ��
	void Initialize(class GLFWwindow* Window);

	// ����״̬
	void Tick();

public:
	// ���̰����Ƿ��ڰ���״̬
	bool IsKeyPressed(int Key) const;

	// ��갴���Ƿ��ڰ���״̬
	bool IsMouseButtonPressed(int Button) const;

	// ��ȡ���λ��
	MyMath::FVector2 GetMousePosition() const;

	// ��ȡ���ƫ����(�����ǰһ֡)
	MyMath::FVector2 GetMouseOffset() const;

	// ��ȡ����ƫ�����������ǰһ֡��
	double GetScrollOffset() const;

private:
	MInputManager() = default;

	// ���̻ص�
	static void KeyCallback(class GLFWwindow* window, int key, int scancode, int action, int mods);

	// ��갴���ص�
	static void MouseButtonCallback(class GLFWwindow* window, int button, int action, int mods);

	// ���λ�ûص�
	static void CursorPosCallback(class GLFWwindow* window, double xpos, double ypos);

	// �����ֻص�
	static void ScrollCallback(class GLFWwindow* window, double xoffset, double yoffset);

private:
	// glfw����
	class GLFWwindow* Window_ = nullptr;

	// ����״̬
	MySTL::THashTable<int, int> KeyStates_;

	// ���״̬
	MySTL::THashTable<int, int> MouseButtonStates_;
	double MouseX_ = 0.0;
	double MouseY_ = 0.0;
	double MouseOffsetX_ = 0.0;
	double MouseOffsetY_ = 0.0;
	double ScrollOffset_ = 0.0;
};
