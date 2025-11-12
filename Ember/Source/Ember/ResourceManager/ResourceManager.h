// ResourceManager.h
// 资源管理器
// created by FirstRay2001, Nov/9/2025

#pragma once

#include "Ember/Core/Core.h"
#include "Ember/Core/Concurrent/AsyncTask.h"
#include "Resource.h"

namespace Ember
{
	class ResourceManager
	{
	public:
		// 单例初始化
		static void Init(std::function<void()> injectInit = nullptr, std::function<void()> injectCleanup = nullptr)
		{
			if (s_Instance == nullptr)
			{
				s_Instance = new ResourceManager(1, injectInit, injectCleanup);	// 只能有一个GPU资源加载线程
			}
			else
			{
				EMBER_CORE_ERROR("ResourceManager already initialized!");
				EMBER_CORE_ASSERT(false, "ResourceManager already initialized!");
			}
		}

		// 单例获取
		static ResourceManager& Get()
		{
			if (s_Instance == nullptr)
			{
				EMBER_CORE_ERROR("ResourceManager not initialized! Call ResourceManager::Init() first.");
				EMBER_CORE_ASSERT(false, "ResourceManager not initialized!");
			}
			return *s_Instance;
		}

	public:
		// 异步加载Shader
		std::future<ResourceHandle> LoadShaderAsync(const std::string& path,
			const ShaderProperties& properties, std::function<void(ResourceHandle)> callback = nullptr);

		// 异步加载纹理
		std::future<ResourceHandle> LoadTextureAsync(const std::string& path,
			const TextureProperties& properties, std::function<void(ResourceHandle)> callback = nullptr);

		// 获取资源
		Ref<IResource> GetResource(ResourceHandle handle) const
		{
			std::lock_guard<std::mutex> lock(m_ResourceMutex);
			auto it = m_Resources.find(handle);
			if (it != m_Resources.end())
			{
				return it->second;
			}
			return nullptr;
		}

	private:
		template<typename ResourceType, typename... Args>
		ResourceHandle LoadResourceInternal(Args&&... args)
		{
			auto resource = CreateScope<ResourceType>(std::forward<Args>(args)...);
			resource->SetHandle(GenerateHandle());

			// 加载资源
			if (resource->Load())
			{
				// 上锁
				std::lock_guard<std::mutex> lock(m_ResourceMutex);

				// 存储资源
				auto handle = resource->GetHandle();
				m_Resources[handle] = std::move(resource);
				return handle;
			}
			else
			{
				EMBER_CORE_ERROR("Failed to load resource: {}", resource->GetPath());
				return NULL_RESOURCE;
			}

		}

		ResourceHandle GenerateHandle()
		{
			static std::atomic<ResourceHandle> s_NextHandle{ 1 };
			return s_NextHandle++;
		}

		void Cleanup()
		{
			std::lock_guard<std::mutex> lock(m_ResourceMutex);
			for (auto res : m_Resources)
			{
				// 卸载资源
				res.second->Unload();
			}
			m_Resources.clear();
			EMBER_CORE_TRACE("ResourceManager destroyed and all resources unloaded.");
		}

	private:
		// 单例
		ResourceManager(size_t threadCount, std::function<void()> injectInit, std::function<void()> injectCleanup) :
			m_TaskSystem(threadCount, injectInit, injectCleanup)
		{
			if (s_Instance != nullptr)
			{
				EMBER_CORE_ERROR("ResourceManager already initialized!");
				EMBER_CORE_ASSERT(false, "ResourceManager already initialized!");
				delete this;
			}
			else
			{
				s_Instance = this;
				EMBER_CORE_TRACE("ResourceManager initialized with {} threads.", threadCount);
			}
		}

		// 单例
		~ResourceManager()
		{
			Cleanup();
		}

	private:
		AsyncTaskSystem<ResourceHandle>							m_TaskSystem;
		std::unordered_map<ResourceHandle, Ref<IResource>>		m_Resources;			// 资源存储表 临界资源
		mutable std::mutex										m_ResourceMutex;

		// 单例
		static ResourceManager*									s_Instance;
	};
}
