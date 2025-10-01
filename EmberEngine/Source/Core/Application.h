// Application.h
// Ӧ�ó�������
// created by FirstRay2001, Sep/15/2025

#pragma once

#include "Common.h"

class Application {
public:
	Application(int Width = 800, int Height = 600, const char* title = "Default Window");
	~Application();

public:
	void Run();
	void SetScene(FScene* Scene);

private:
	// ���ڳ�ʼ��
	void Initialize();

	// ��ѭ��
	void MainLoop();

	// ��������
	void Terminate();

private:
	void HandleInput();

	// ���ڴ�С�����ص�
	static void FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height);

private:
	// TODO: ʹ������ָ��
	GLFWwindow* Window_;
	FScene*		CurrentScene_;
	int			WindowWidth_;
	int			WindowHeight_;
	const char*	Titile_;
	MySTL::TUniquePtr<FMesh>		Mesh_;
	MySTL::TUniquePtr<FShader>	Shader_;
};