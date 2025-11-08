// GPUResourceLoadQueue.h
// GPU资源加载队列
// created by FirstRay2001, Nov/8/2025

#pragma once

#include "Ember/Core/Concurrent/CommandQueue.h"
#include <future>

namespace Ember
{
	class Shader;
	class Texture;

	// GPU资源加载命令基类
	class GPUResourceLoadCommand : public ICommand
	{
		friend class GPUResourceLoader;
	protected:
		using CallbackFunc = std::function<void(Ref<Shader>)>;

	public:
		virtual void Execute() override {};

		// 获取加载状态
		bool IsCompleted() const { return m_Completed; }

		// 等待加载完成
		virtual void Wait() {};
	protected:
		std::atomic<bool> m_Completed{ false };
	};

	// Shader加载命令
	class ShaderLoadCommand :public GPUResourceLoadCommand
	{
	public:
		ShaderLoadCommand(const std::string& filepath, CallbackFunc callback = nullptr);

		virtual void Execute() override;

		Ref<Shader> GetResult() const { return m_Future._Get_value(); }
		std::future<Ref<Shader>> GetFuture() { return std::move(m_Future); }

		virtual void Wait() override;

	private:
		std::string m_Filepath;
		CallbackFunc m_Callback;

		Ref<Shader> m_Shader;
		std::promise<Ref<Shader>> m_Promise;
		std::future<Ref<Shader>> m_Future = m_Promise.get_future();
	};

	// GPU资源异步加载器
	class GPUResourceLoader : public CommandQueue<GPUResourceLoadCommand>
	{
	public:
		// 异步加载Shader
		std::future<Ref<Shader>> LoadShaderAsync(const std::string& filepath, ShaderLoadCommand::CallbackFunc callback = nullptr);

		// 清理已完成的加载命令
		void CleanupCompletedCommands();

		// 等待所有加载命令完成
		void WaitForAll();

		// 获取待处理的加载命令数量
		size_t GetPendingCount() const { return m_PendingCommands.size(); }

		// 是否有待处理的加载命令
		bool HasPendiongLoads() const { return !m_PendingCommands.empty(); }

		void SetupWindow(void* window);

	protected:
		virtual void InitThread() override;

		virtual void CleanupThread() override;

	private:
		std::vector<Ref<GPUResourceLoadCommand>> m_PendingCommands;
		void* m_SharedWindow{ nullptr };
		std::atomic<bool> m_Initilized{ false };
	};
}
