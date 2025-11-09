// CommmandQueue.h
// 双缓冲命令队列
// created by FirstRay2001, Nov/8/2025

#pragma once
#include "Ember/Core/Core.h"

#define EMBER_ENABLE_COMMAND_QUEUE_LOG 0

namespace Ember
{
	// 命令接口
	class ICommand
	{
	public:
		virtual void Execute() = 0;
	};

	// 通用函数命令
	class FunctionCommand : public ICommand
	{
	public:
		template<typename Func>
		FunctionCommand(Func&& func) : 
			m_Function(std::forward<Func>(func)) 
		{
		}

		virtual void Execute() override
		{
			m_Function();
		}
	
	private:
		std::function<void()> m_Function;
	};

	// 双缓冲命令队列
	template<typename CommandType>
	class CommandQueue
	{
		// CommandType必须继承自ICommand
		static_assert(std::is_base_of_v<ICommand, CommandType>, "CommandType must be derived from ICommand");

	public:
		virtual ~CommandQueue() = default;

		void Start()
		{
			// 启动消费者线程
			m_ConsumerThread = std::thread(&CommandQueue::MainLoop, this);

#if EMBER_ENABLE_COMMAND_QUEUE_LOG
			EMBER_CORE_TRACE("CommandQueue initialized. Consumer thread started.");
#endif // EMBER_ENABLE_COMMAND_QUEUE_LOG


			if(m_ConsumerThread.joinable())
				m_ConsumerThread.detach();
		}

		void Shutdown()
		{
			// 安全关闭线程
			if (m_ConsumerThread.joinable())
			{
				m_ConsumerThread.join();

#if EMBER_ENABLE_COMMAND_QUEUE_LOG
				EMBER_CORE_TRACE("CommandQueue shutdown.");
#endif // EMBER_ENABLE_COMMAND_QUEUE_LOG
			}
		}

		// 生产者 - 推入命令到写入缓冲区
		void PushCommand(Ref<CommandType>& command)
		{
			std::unique_lock<std::mutex> lock(m_SwapMutex);				// 写入期间不允许交换
			m_CommandBuffers[m_WriteIndex].emplace_back(std::move(command));

#if EMBER_ENABLE_COMMAND_QUEUE_LOG
			EMBER_CORE_TRACE("Command pushed.");
#endif // EMBER_ENABLE_COMMAND_QUEUE_LOG
		}

		// 生产者 - 构造命令到写入缓冲区
		template<typename... Args>
		void EmplaceCommand(Args&&... args)
		{
			std::unique_lock<std::mutex> lock(m_SwapMutex);				// 写入期间不允许交换
			m_CommandBuffers[m_WriteIndex].emplace_back(CreateRef<CommandType>(std::forward<Args>(args)...));

#if EMBER_ENABLE_COMMAND_QUEUE_LOG
			EMBER_CORE_TRACE("Command emplaced.");
#endif // EMBER_ENABLE_COMMAND_QUEUE_LOG
		}

		// 生产者 - 交换读写缓冲区
		void SwapBuffers()
		{
			{
				std::unique_lock<std::mutex> lock(m_SwapMutex);
				m_ReadIndex = 1 - m_ReadIndex.load();
				m_WriteIndex = 1 - m_WriteIndex.load();
			}
			m_DataReadyCV.notify_one();									// 通知消费者有数据可读

#if EMBER_ENABLE_COMMAND_QUEUE_LOG
			EMBER_CORE_TRACE("Command buffer swapped.");
#endif // EMBER_ENABLE_COMMAND_QUEUE_LOG
		}

		// 生产者 - 等待消费者完成命令处理
		void WaitForCompletion()
		{
			std::unique_lock<std::mutex> lock(m_SwapMutex);

			if (m_CommandBuffers[m_ReadIndex].empty())
			{

#if EMBER_ENABLE_COMMAND_QUEUE_LOG
				EMBER_CORE_TRACE("No need to wait.");
#endif // EMBER_ENABLE_COMMAND_QUEUE_LOG

				return;
			}

#if EMBER_ENABLE_COMMAND_QUEUE_LOG
			EMBER_CORE_TRACE("Wait for command compelete.");
#endif // EMBER_ENABLE_COMMAND_QUEUE_LOG

			m_CommandCompletedCV.wait(lock, [this]() {
				return m_CommandBuffers[m_ReadIndex].empty();
				});
		}

	protected:
		// 消费者 - 线程主循环
		void MainLoop()
		{
			InitThread();

			while (true)
			{
				std::unique_lock<std::mutex> lock(m_SwapMutex);
				m_DataReadyCV.wait(lock, [this]() {
					return !m_CommandBuffers[m_ReadIndex].empty();
					});

				// 处理命令
				auto& commandBuffer = m_CommandBuffers[m_ReadIndex];
				for (auto& command : commandBuffer)
				{
					// 执行命令
					command->Execute();
				}
				commandBuffer.clear();

				// 通知生产者命令已完成
				m_CommandCompletedCV.notify_all();

#if EMBER_ENABLE_COMMAND_QUEUE_LOG
				EMBER_CORE_TRACE("All command compeleted.");
#endif // EMBER_ENABLE_COMMAND_QUEUE_LOG
			}

			CleanupThread();
		}

	protected:
		// 线程内初始化
		virtual void InitThread() {};

		// 线程内清理
		virtual void CleanupThread() {};

	protected:
		std::thread					m_ConsumerThread;
		std::vector<Ref<CommandType>>	m_CommandBuffers[2];
		std::atomic<int>			m_ReadIndex{ 0 };
		std::atomic<int>			m_WriteIndex{ 1 };
		std::mutex					m_SwapMutex;
		std::condition_variable		m_DataReadyCV;
		std::condition_variable		m_CommandCompletedCV;
	};
}