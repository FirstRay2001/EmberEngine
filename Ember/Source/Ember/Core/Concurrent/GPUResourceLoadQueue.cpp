#pragma once

#include "emberpch.h"
#include "GPUResourceLoadQueue.h"
#include "Ember/Renderer/Shader.h"

#include "GLFW/glfw3.h"
#include "Ember/Core/Application.h"

namespace Ember
{
	ShaderLoadCommand::ShaderLoadCommand(const std::string& filepath, CallbackFunc callback) :
		m_Filepath(filepath),
		m_Callback(callback)
	{
	}

	void ShaderLoadCommand::Execute()
	{
		auto threadId = std::this_thread::get_id();
		EMBER_CORE_TRACE("Loading Shader({0}) in thread({1})",
			m_Filepath.c_str(), std::to_string(std::hash<std::thread::id>{}(threadId)));

		try
		{
			m_Shader = Shader::Create(m_Filepath);

			if(m_Callback)
				m_Callback(m_Shader);
		}
		catch (const std::exception& e)
		{
			EMBER_CORE_ERROR("Failed to load Shader({0}): {1}", m_Filepath.c_str(), e.what());
			m_Promise.set_value(nullptr);
			m_Completed = true;
			return;
		}

		m_Promise.set_value(m_Shader);
		m_Completed = true;
	}

	void ShaderLoadCommand::Wait()
	{
		m_Future.wait();
	}

	std::future<Ref<Shader>> GPUResourceLoader::LoadShaderAsync(const std::string& filepath, ShaderLoadCommand::CallbackFunc callback)
	{
		auto command = CreateRef<ShaderLoadCommand>(filepath, callback);

		// 将命令添加到队列
		PushCommand(std::static_pointer_cast<GPUResourceLoadCommand>(command));
		m_PendingCommands.push_back(command);

		return command->GetFuture();
	}

	void GPUResourceLoader::CleanupCompletedCommands()
	{
		m_PendingCommands.erase(
			std::remove_if(
				m_PendingCommands.begin(),
				m_PendingCommands.end(),
				[](const Ref<GPUResourceLoadCommand>& cmd) { return cmd->IsCompleted(); }
			),
			m_PendingCommands.end()
		);
	}

	void GPUResourceLoader::WaitForAll()
	{
		for (auto& command : m_PendingCommands)
		{
			command->Wait();
		}
	}

	void GPUResourceLoader::SetupWindow(void* window)
	{
		m_SharedWindow = (void*)glfwCreateWindow(1, 1, "", nullptr, (GLFWwindow*)window);
		m_Initilized = true;
	}

	void GPUResourceLoader::InitThread()
	{
		// TODO: 平台无关处理
		if (!m_Initilized.load())
		{
			EMBER_CORE_ERROR("GPUResourceLoader not initialized with a shared window!");
			EMBER_CORE_ASSERT(false, "GPUResourceLoader not initialized with a shared window!");
		}
		glfwMakeContextCurrent((GLFWwindow*)m_SharedWindow);
	}

	void GPUResourceLoader::CleanupThread()
	{
		// TODO: 平台无关处理
		glfwDestroyWindow((GLFWwindow*)m_SharedWindow);
		m_SharedWindow = nullptr;
	}
}