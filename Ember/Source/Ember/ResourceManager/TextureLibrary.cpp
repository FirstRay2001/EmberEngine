// TextureLibrary.cpp
// 纹理管理器
// created by FirstRay2001, Nov/11/2025

#include "emberpch.h"
#include "TextureLibrary.h"

namespace Ember
{
	void TextureLibrary::Add(const std::string& name, const Ref<Texture2D>& texture)
	{
		if(Exists(name))
			EMBER_CORE_WARN("Texture already exists! Overwriting {0}", name.c_str());
		m_Textures[name] = texture;
	}

	Ref<Texture2D> TextureLibrary::LoadSync(const std::string& filepath)
	{
		auto texture = Texture2D::Create(filepath);
		auto name = Texture::FilePathToName(filepath);
		Add(name, texture);
		return texture;
	}

	void TextureLibrary::LoadAsync(const std::string& filepath)
	{
		auto textureFuture = ResourceManager::Get().LoadTextureAsync(filepath, TextureProperties(), [this](ResourceHandle handle)
			{
				EMBER_CORE_TRACE("Texture loaded with handle: {}", handle);
			});
		auto name = Texture::FilePathToName(filepath);

		// Note - 很关键的知识点
		//	   - 首先，future不能拷贝，只能移动，所以使用move
		//	   - 其次，使用insert + make_pair的方式避免unordered_map构造时的拷贝
		m_LoadingTextures.insert(std::make_pair(name, std::move(textureFuture)));
	}

	// Note - 不在子线程中创建GPU纹理, 仅在子线程中将数据加载到内存
	//		- 在主线程中创建GPU纹理
	//		- 如果不这样做，会出现奇怪的纹理创建异常
	Ref<Texture2D> TextureLibrary::GetTextureSync(const std::string& name)
	{
		if (Exists(name))
			return m_Textures[name];

		// 如果纹理正在加载，等待其完成
		auto it = m_LoadingTextures.find(name);
		if (it != m_LoadingTextures.end())
		{
			auto handle = it->second.get();
			auto resource = ResourceManager::Get().GetResource(handle);

			// 提取数据
			auto data = std::dynamic_pointer_cast<TextureResource>(resource)->GetTextureData();

			// 上传到GPU
			auto prop = std::dynamic_pointer_cast<TextureResource>(resource)->GetProperties();
			auto texture = Texture2D::Create(data, prop.Width, prop.Height, prop.Channels);

			// 缓存纹理
			Add(name, texture);
			m_LoadingTextures.erase(it);
			return texture;
		}
		EMBER_CORE_ASSERT(false, "Texture not found: {}", name);
		return nullptr;
	}

	Ref<Texture2D> TextureLibrary::GetTextureAsync(const std::string& name)
	{
		if (Exists(name))
			return m_Textures[name];

		// 检查该Texture是否加载中
		auto it = m_LoadingTextures.find(name);
		if (it != m_LoadingTextures.end())
		{
			// 非阻塞获取内存data
			auto status = it->second.wait_for(std::chrono::seconds(0));
			if (status == std::future_status::ready)
			{
				auto handle = it->second.get();
				auto resource = ResourceManager::Get().GetResource(handle);

				// 提取数据
				auto data = std::dynamic_pointer_cast<TextureResource>(resource)->GetTextureData();

				// 上传到GPU
				auto prop = std::dynamic_pointer_cast<TextureResource>(resource)->GetProperties();
				auto texture = Texture2D::Create(data, prop.Width, prop.Height, prop.Channels);
				
				// 缓存纹理
				Add(name, texture);
				m_LoadingTextures.erase(it);
				return texture;
			}
			else
			{
				EMBER_CORE_WARN("Texture {0} is still loading.", name.c_str());
			}
		}
		else
		{
			EMBER_CORE_ERROR("Texture not found! {0}", name.c_str());
		}
		return nullptr;
	}

	bool TextureLibrary::Exists(const std::string& name) const
	{
		return m_Textures.find(name) != m_Textures.end();
	}
}
