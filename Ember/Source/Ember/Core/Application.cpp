// Application.cpp
// 应用类
// created by FirstRay2001, Oct/30/2025

#include "emberpch.h"
#include "Ember/Core/Application.h"
#include "Ember/Events/ApplicationEvent.h"
#include "Ember/Events/KeyEvent.h"
#include "Ember/ImGui/ImGuiLayer.h"
#include "Ember/Renderer/Renderer.h"
#include "Ember/Scripting/ScriptEngine.h"
#include "Ember/Core/Timestep.h"
#include "Ember/ResourceManager/ResourceManager.h"

#include "Ember/Renderer/Animation/Animation.h"

#include <GLFW/glfw3.h>

#include <cstdlib>

#include <filesystem>

using namespace Ember;

Application* Application::s_Instance = nullptr;

Application::Application()
{
	system("pause");

	// 输出当前工作目录
	EMBER_CORE_TRACE("Current working directory: {}", std::filesystem::current_path().string());

	EMBER_CORE_ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	m_Window = Scope<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	// 初始化多线程资源加载系统
	// Note - 这里需要创建一个共享的OpenGL上下文，用于资源加载线程
	//		- 因为OpenGL上下文不能跨线程使用，所以需要为资源加载线程创建一个共享上下文
	//		- 主线程（主窗口上下文）  加载线程（以主窗口上下文为基础的上下文）
	//		- 主线程窗口作为共享上下文的基础，这样任何在共享上下文中创建的资源都可以在主线程上下文中使用
	//		- 目前只支持单线程的资源加载
	//		- 扩展思路： 可以创建一个上下文池，供多个资源加载线程使用
	{
		void* nativeWindow = m_Window->GetNativeWindow();
		void* sharedWindow = RenderCommand::SetupMutithread(nativeWindow);	// 生成共享窗口上下文,用于资源异步加载

		// 初始化资源管理器
		std::function injectInit = [sharedWindow]()
			{
				RenderCommand::InitMutiThread(sharedWindow);	// 在线程内，设置共享上下文
			};
		std::function injectCleanup = []()
			{
				RenderCommand::ShutdownMutiThread();			// 在线程内，销毁上下文
			};
		ResourceManager::Init(injectInit, injectCleanup);		// 注入初始化和清理函数，以便在线程内调用
	}

	// 初始化Renderer
	Renderer::Init();

	// 初始化ScriptEngine
	ScriptEngine::Init();

	// 初始化ImGui
	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

}

void Application::Run()
{
	while (m_Running)
	{
		// 更新帧时间
		float time = (float)glfwGetTime(); // 获取当前时间
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		// 逐层更新
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate(timestep);

		// ImGui更新
		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}
		m_ImGuiLayer->End();
		
		// 更新窗口
		m_Window->OnUpdate();
	}
}

void Ember::Application::Close()
{
	m_Running = false;
}

void Ember::Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));
	
	// 自顶向下遍历图层堆栈
	for (auto It = m_LayerStack.end(); It != m_LayerStack.begin(); )
	{
		(*--It)->OnEvent(e);
		if (e.Handled)
			break;
	}
}

bool Ember::Application::OnWindowClose(WindowCloseEvent& e)
{
	Close();
	return true;
}

bool Ember::Application::OnWindowResize(WindowResizeEvent& e)
{
	Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
	return false;
}

bool Ember::Application::OnKeyPressed(KeyPressedEvent& e)
{
	return false;
}

void Ember::Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);

	// 同步窗口大小
	WindowResizeEvent e(m_Window->GetWidth(), m_Window->GetHeight());
	layer->OnEvent(e);
}

void Ember::Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PushOverlay(overlay);
}

ImGuiLayer* Ember::Application::GetImGuiLayer()
{
	return m_ImGuiLayer;
}
