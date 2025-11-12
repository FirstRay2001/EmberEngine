// AsyncTask.h
// 异步任务系统
// created by FirstRay2001, Nov/9/2025

#pragma once

#include "Ember/Core/Core.h"
#include <future>

namespace Ember
{
	// 异步任务系统
	template<typename TaskResult>
	class AsyncTaskSystem
	{
	public:
		using Task = std::function<TaskResult()>;
		using TaskId = uint32_t;

		// 异步任务请求结构体
		struct TaskRequest
		{
			TaskId								Id;		// 任务ID
			Task								TargetTask;		// 任务函数
			std::promise<TaskResult>			Promise;	// 任务结果promise
			std::function<void(TaskResult)>		Callback;	// 任务完成回调

			TaskRequest(Task&& task, std::function<void(TaskResult)> callback = nullptr) :
				TargetTask(std::move(task)),
				Callback(callback)
			{
				static std::atomic<TaskId> s_NextId{ 1 };	// 全局静态自增ID
				Id = s_NextId++;
			}
		};

	public:
		AsyncTaskSystem(size_t threadCount = 1, std::function<void()> injectInit = nullptr, std::function<void()> injectCleanup = nullptr) :
			m_Running(false),
			m_InjectInit(injectInit),
			m_InjectCleanup(injectCleanup)
		{
			Start(threadCount);
		}

		~AsyncTaskSystem()
		{
			Stop();
		}

	public:
		// 提交异步任务
		std::future<TaskResult> Submit(Task&& task, std::function<void(TaskResult)> callback = nullptr)
		{
			auto request = CreateScope<TaskRequest>(std::move(task), callback);
			auto future = request->Promise.get_future();
			
			// 上锁并将任务请求添加到队列
			{
				std::lock_guard<std::mutex> lock(m_QueueMutex);
				m_TaskQueue.push(std::move(request));
			}

			// 唤醒一个工作线程
			m_Condition.notify_one();
			return future;
		}

		// 启动任务系统
		void Start(size_t threadCount = 1)
		{
			// 防止重复启动
			if (m_Running.load())
				return;

			m_Running = true;
			for (size_t i = 0; i < threadCount; ++i)
			{
				m_WorkerThreads.emplace_back(&AsyncTaskSystem::ThreadMainLoop, this);
			}
		}

		// 停止任务系统
		void Stop()
		{
			m_Running = false;
			m_Condition.notify_all();

			// 等待所有工作线程完成
			for (auto& thread : m_WorkerThreads)
			{
				if (thread.joinable())
					thread.join();
			}
			m_WorkerThreads.clear();
		}

		// 获取待处理任务数量
		size_t GetyPendingCount() const
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			return m_TaskQueue.size();
		}

		bool IsRunning() const { return m_Running; }

	private:
		// 工作线程主循环
		void ThreadMainLoop()
		{
			// 注入初始化代码
			if (m_InjectInit)
				m_InjectInit();

			while (true)
			{
				Scope<TaskRequest> taskRequest;

				// 获取任务
				{
					std::unique_lock<std::mutex> lock(m_QueueMutex);
					m_Condition.wait(lock, [this]() {
						return !m_TaskQueue.empty() || !m_Running.load();	// 等待有任务或停止信号
						});

					if (!m_Running.load() && m_TaskQueue.empty())
						return; // 退出线程

					// 获取队列第一个任务
					taskRequest = std::move(m_TaskQueue.front());
					m_TaskQueue.pop();
				}

				// 执行任务
				try
				{
					TaskResult result = taskRequest->TargetTask();	// 执行任务
					taskRequest->Promise.set_value(result);	// 设置结果

					// 调用回调函数
					if (taskRequest->Callback)
					{
						taskRequest->Callback(result);		// 执行回调
					}
				}
				catch (const std::exception& e)
				{
					EMBER_CORE_ERROR("AsyncTask Exception: {0}", e.what());

					// 设置异常到promise
					taskRequest->Promise.set_exception(std::current_exception());
				}
			}

			// 注入清理代码
			if (m_InjectCleanup)
				m_InjectCleanup();
		}

	private:
		std::vector<std::thread>		m_WorkerThreads;
		std::queue<Scope<TaskRequest>>	m_TaskQueue;				// 任务队列 临界资源
		mutable std::mutex				m_QueueMutex;				// 声明为mutable以允许const方法锁定
		std::condition_variable			m_Condition;
		std::atomic<bool>				m_Running;
		std::function<void()>			m_InjectInit;
		std::function<void()>			m_InjectCleanup;
	};
}
