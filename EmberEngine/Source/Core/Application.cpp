#include "Application.h"
// Application.cpp
// 应用程序主类
// created by FirstRay2001, Sep/15/2025

#include "Application.h"
#include "Source/Scene/Scene.h"
#include "Source/Manager/InputManager.h"
#include "Source/Manager/TextureManager.h"
#include "Source//Manager/ShaderManager.h"
#include "Source/Manager/ModelManager.h"
#include "Source/Manager/AnimationManager.h"
#include "Source/Manager/DebugDrawManager.h"

Application::Application(int Width, int Height, const char* title) :
	Window_(nullptr),
	CurrentScene_(nullptr),
	WindowWidth_(Width),
	WindowHeight_(Height),
	Titile_(title),
	DeltaTime_(0.0f),
	LastFrameTime_(0.0f),
	CurrentFrameTime_(0.0f)
{
	Initialize();
}

Application::~Application()
{
}

void Application::Run()
{
	IfFailGo(Window_ != NULL);

	// 载入场景
	CurrentScene_ = MySTL::TSharedPtr<FScene>(new FScene());
	CurrentScene_->Load();

	// 启动循环
	MainLoop();

	// 结束
	CurrentScene_->Unload();
	Terminate();
}

void Application::SetScene(FScene* Scene)
{
}

void Application::Initialize()
{
	// 初始化GLFW
	if (!glfwInit())
	{
		LOG_ERROR("Failed to initialze GLFW");
		return;
	}

	// 设置OpenGL版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
	Window_ = glfwCreateWindow(WindowWidth_, WindowHeight_, Titile_, NULL, NULL);
	if (Window_ == NULL)
	{
		LOG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return;
	}

	// 设置窗口上下文
	glfwMakeContextCurrent(Window_);

	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
		return;
	}

	// 设置窗口属性
	glViewport(0, 0, WindowWidth_, WindowHeight_);
	glfwSetFramebufferSizeCallback(Window_, FrameBufferSizeCallback);

	LOG_INFO("Window intialized: %s", Titile_);

	// 初始化帧时间
	LastFrameTime_ = glfwGetTime();

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);

	// 启用混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 管理器初始化
	MInputManager::GetInstance().Initialize(Window_);
	MTextureManager::GetInstance().Initialize();
	MModelManager::GetInstance().Initialize();
	MShaderManager::GetInstance().Initialize();
	MAnimationManager::GetInstance().Initialize();
	MDebugDrawManager::GetInstance().Initialize();

	// 捕获鼠标
	glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(Window_))
	{
		//////// 处理输入 ////////
		ShouldClose();
		
		//////// 场景逻辑 ////////
		// 计算帧间隔
		CurrentFrameTime_ = glfwGetTime();
		DeltaTime_ = CurrentFrameTime_ - LastFrameTime_;
		LastFrameTime_ = CurrentFrameTime_;

		// 驱动场景
		CurrentScene_->Tick(DeltaTime_);

		//////// 渲染场景 ////////
		// 渲染
		CurrentScene_->Render();

		// 交换颜色缓冲
		glfwSwapBuffers(Window_);

		//////// Post Tick ////////
		MInputManager::GetInstance().Tick();
		MTextureManager::GetInstance().Tick();
		MDebugDrawManager::GetInstance().Tick();
		MAnimationManager::GetInstance().Tick(DeltaTime_);

		glfwPollEvents();
	}
}

void Application::Terminate()
{
	glfwDestroyWindow(Window_);
	glfwTerminate();
}

void Application::ShouldClose()
{
	// 按下ESC关闭窗口
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(Window_, true);
		glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// 按M捕获鼠标
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_M))
	{
		glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// 按N释放鼠标
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_N))
	{
		glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Application::FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}
