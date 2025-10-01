// Application.cpp
// 应用程序主类
// created by FirstRay2001, Sep/15/2025

#include "Application.h"

#include "Source/Graphics/Mesh.h"
#include "Source/Graphics/Shader.h"
#include "Source/Graphics/Texture.h"

Application::Application(int Width, int Height, const char* title) :
	Window_(nullptr),
	CurrentScene_(nullptr),
	WindowWidth_(Width),
	WindowHeight_(Height),
	Titile_(title)
{
	Initialize();
}

Application::~Application()
{
}

void Application::Run()
{
	IfFailGo(Window_ != NULL);

	MainLoop();
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

	// 设置清屏颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// test
	FVertex
		ver1{ MyMath::FVector3(-0.5, -0.5, 0.0), MyMath::FVector3(1,1,1), MyMath::FVector2(0,0) },
		ver2{ MyMath::FVector3(0.5, -0.5, 0.0), MyMath::FVector3(1,1,1), MyMath::FVector2(1,0) },
		ver3{ MyMath::FVector3(0.5,  0.5, 0.0), MyMath::FVector3(1,1,1), MyMath::FVector2(1,1) },
		ver4{ MyMath::FVector3(-0.5,  0.5, 0.0), MyMath::FVector3(1,1,1), MyMath::FVector2(0,1) };
	MySTL::TVector<FVertex> Vertices;
	Vertices.push_back(ver1);
	Vertices.push_back(ver2);
	Vertices.push_back(ver3);
	Vertices.push_back(ver4);

	MySTL::TVector<unsigned int> Indices;
	Indices.push_back(0);
	Indices.push_back(1);
	Indices.push_back(2);
	Indices.push_back(0);
	Indices.push_back(3);
	Indices.push_back(2);

	Mesh_ = MySTL::TUniquePtr<FMesh>(new FMesh(Vertices, Indices));

	const char* VertexPath = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Shader/TestShader/testvs.vert";
	const char* FragmentPath = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Shader/TestShader/testfs.frag";
	Shader_ = MySTL::TUniquePtr<FShader>(new FShader(VertexPath, FragmentPath));

	const char* PicPath = "./Resources/Texture/awesomeface.png";
	FTexture tex(PicPath);

}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(Window_))
	{
		//////// 处理输入 ////////
		HandleInput();


		//////// 渲染 ////////

		// 清屏
		glClear(GL_COLOR_BUFFER_BIT);

		Shader_->Use();
		Mesh_->Draw();
		glfwSwapBuffers(Window_);


		glfwPollEvents();
	}
}

void Application::Terminate()
{
	glfwDestroyWindow(Window_);
	glfwTerminate();
}

void Application::HandleInput()
{
	// 按下ESC关闭窗口
	if (glfwGetKey(Window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Window_, true);
}

void Application::FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}
