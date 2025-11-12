// ResourceManager.cpp
// 资源管理器
// created by FirstRay2001, Nov/9/2025

#include "emberpch.h"
#include "ResourceManager.h"

namespace Ember
{
	ResourceManager* ResourceManager::s_Instance = nullptr;

	std::future<ResourceHandle> ResourceManager::LoadShaderAsync(const std::string& path, const ShaderProperties& properties, std::function<void(ResourceHandle)> callback)
	{
		return m_TaskSystem.Submit(
			[this, path, properties]() -> ResourceHandle
			{
				return LoadResourceInternal<ShaderResource>(path, properties);
			},
			callback
		);
	}

	std::future<ResourceHandle> ResourceManager::LoadTextureAsync(const std::string& path, const TextureProperties& properties, std::function<void(ResourceHandle)> callback)
	{
		return m_TaskSystem.Submit(
			[this, path, properties]() -> ResourceHandle
			{
				return LoadResourceInternal<TextureResource>(path, properties);
			},
			callback
		);
	}
}