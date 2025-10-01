// Application.h
// 应用程序主类
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
	// 窗口初始化
	void Initialize();

	// 主循环
	void MainLoop();

	// 结束程序
	void Terminate();

private:
	void HandleInput();

	// 窗口大小调整回调
	static void FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height);

private:
	// TODO: 使用智能指针
	GLFWwindow* Window_;
	FScene*		CurrentScene_;
	int			WindowWidth_;
	int			WindowHeight_;
	const char*	Titile_;
	MySTL::TUniquePtr<FMesh>		Mesh_;
	MySTL::TUniquePtr<FShader>	Shader_;
};