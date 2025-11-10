// Resource.h
// 资源类
// created by FirstRay2001, Nov/9/2001

#include "emberpch.h"
#include "Resource.h"
#include "Ember/Renderer/Shader.h"
#include "Ember/Renderer/Texture.h"

namespace Ember
{
	ShaderResource::ShaderResource(const std::string& path, const ShaderProperties& props) :
		m_Properties(props)
	{
		m_Path = path;
	}

	bool ShaderResource::Load()
	{
		m_Shader = Shader::Create(m_Path);
		if (m_Shader)
		{
			EMBER_CORE_INFO("Shader resource loaded: {}", m_Path);
			return true;
		}
		else
		{
			EMBER_CORE_ERROR("Failed to load shader resource: {}", m_Path);
			return false;
		}
	}

	void ShaderResource::Unload()
	{
		// Do nothing for now
	}

	TextureResource::TextureResource(const std::string& path, const TextureProperties& props) :
		m_Properties(props),
		m_TextureData(nullptr)
	{
		m_Path = path;
	}

	bool TextureResource::Load()
	{
		m_TextureData = Texture2D::ReadData(m_Path, m_Properties.m_Width, m_Properties.m_Height, m_Properties.m_Channels);
		if (m_TextureData != nullptr)
		{
			EMBER_CORE_INFO("Texture resource loaded: {}", m_Path);
			return true;
		}
		else
		{
			EMBER_CORE_ERROR("Failed to load texture resource: {}", m_Path);
			return false;
		}
	}

	void TextureResource::Unload()
	{
		// 释放纹理内存
		Texture2D::freeImageData(m_TextureData);
	}
}