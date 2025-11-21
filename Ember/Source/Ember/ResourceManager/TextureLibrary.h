// TextureLibrary.h
// 纹理管理器
// created by FirstRay2001, Nov/11/2025

#pragma once

#include "Ember/Core/Core.h"
#include "Ember/ResourceManager/ResourceManager.h"
#include "Ember/Renderer/Texture.h"

namespace Ember
{
	class TextureLibrary
	{
	public:
		// 单例
		static TextureLibrary& Get()
		{
			static TextureLibrary instance;
			return instance;
		}

		// 添加纹理
		void Add(const std::string& name, const Ref<Texture2D>& texture);

		// 同步加载纹理
		Ref<Texture2D> LoadSync(const std::string& filepath);

		// 异步加载纹理
		void LoadAsync(const std::string& filepath);

		// 立即获取纹理
		Ref<Texture2D> GetTextureSync(const std::string& name);

		// 非阻塞获取纹理
		Ref<Texture2D> GetTextureAsync(const std::string& name);


		bool Exists(const std::string& name) const;

	private:
		TextureLibrary()	= default;
		~TextureLibrary()	= default;

	private:
		std::unordered_map<std::string, std::future<ResourceHandle>> m_LoadingTextures;
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
	};
}