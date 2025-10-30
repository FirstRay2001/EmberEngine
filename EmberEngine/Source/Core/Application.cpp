#include "Application.h"
// Application.cpp
// Ӧ�ó�������
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

	// ���볡��
	CurrentScene_ = MySTL::TSharedPtr<FScene>(new FScene());
	CurrentScene_->Load();

	// ����ѭ��
	MainLoop();

	// ����
	CurrentScene_->Unload();
	Terminate();
}

void Application::SetScene(FScene* Scene)
{
}

void Application::Initialize()
{
	// ��ʼ��GLFW
	if (!glfwInit())
	{
		LOG_ERROR("Failed to initialze GLFW");
		return;
	}

	// ����OpenGL�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��������
	Window_ = glfwCreateWindow(WindowWidth_, WindowHeight_, Titile_, NULL, NULL);
	if (Window_ == NULL)
	{
		LOG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return;
	}

	// ���ô���������
	glfwMakeContextCurrent(Window_);

	// ��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
		return;
	}

	// ���ô�������
	glViewport(0, 0, WindowWidth_, WindowHeight_);
	glfwSetFramebufferSizeCallback(Window_, FrameBufferSizeCallback);

	LOG_INFO("Window intialized: %s", Titile_);

	// ��ʼ��֡ʱ��
	LastFrameTime_ = glfwGetTime();

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// ���û��
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ��������ʼ��
	MInputManager::GetInstance().Initialize(Window_);
	MTextureManager::GetInstance().Initialize();
	MModelManager::GetInstance().Initialize();
	MShaderManager::GetInstance().Initialize();
	MAnimationManager::GetInstance().Initialize();
	MDebugDrawManager::GetInstance().Initialize();

	// �������
	glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(Window_))
	{
		//////// �������� ////////
		ShouldClose();
		
		//////// �����߼� ////////
		// ����֡���
		CurrentFrameTime_ = glfwGetTime();
		DeltaTime_ = CurrentFrameTime_ - LastFrameTime_;
		LastFrameTime_ = CurrentFrameTime_;

		// ��������
		CurrentScene_->Tick(DeltaTime_);

		//////// ��Ⱦ���� ////////
		// ��Ⱦ
		CurrentScene_->Render();

		// ������ɫ����
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
	// ����ESC�رմ���
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(Window_, true);
		glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// ��M�������
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_M))
	{
		glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// ��N�ͷ����
	if (MInputManager::GetInstance().IsKeyPressed(GLFW_KEY_N))
	{
		glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Application::FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}
