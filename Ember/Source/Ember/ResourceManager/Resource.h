// Resource.h
// 资源类
// created by FirstRay2001, Nov/9/2001

#pragma once

#include "Ember/Core/Core.h"

namespace Ember
{
	// 资源句柄
	using ResourceHandle = uint32_t;
	constexpr ResourceHandle NULL_RESOURCE = 0;

	// 资源状态
	enum class ResourceState
	{
		Unloaded = 0,
		Loading,
		Ready,
		Error
	};

	// 资源类型
	enum class ResourceType
	{
		Texture,
		Shader,
		Model
	};

	// 纹理属性

	// Shader属性
	struct ShaderProperties
	{
	};

	// 纹理属性
	struct TextureProperties
	{
		int m_Width = 0;
		int m_Height = 0;
		int m_Channels = 0;
	};

	// 资源接口
	class IResource
	{
	public:
		virtual ~IResource() = default;

		// 设置资源句柄
		void SetHandle(ResourceHandle handle) { m_Handle = handle; }

		// 获取资源句柄
		ResourceHandle GetHandle() const { return m_Handle; }

		// 获取资源名称
		std::string GetName() const { return m_Name; }

		// 获取资源类型
		ResourceType GetType() const { return m_Type; }

		// 获取资源路径
		const std::string& GetPath() const { return m_Path; }

		// 获取资源状态
		ResourceState GetState() const { return m_State; }

		virtual bool Load() = 0;
		virtual void Unload() = 0;

	protected:
		std::string			m_Name;
		ResourceType		m_Type;
		std::string			m_Path;
		ResourceHandle		m_Handle = NULL_RESOURCE;
		ResourceState		m_State = ResourceState::Unloaded;
	};

	class Shader;

	// Shader资源
	class ShaderResource : public IResource
	{
	public:
		ShaderResource(const std::string& path, const ShaderProperties& props);

		virtual bool Load() override;
		virtual void Unload() override;

		Ref<Shader> GetShader() const { return m_Shader; }
	
	private:
		ShaderProperties	m_Properties;
		Ref<Shader>			m_Shader;
	};

	class Texture2D;

	// 纹理资源
	class TextureResource : public IResource
	{
	public:
		TextureResource(const std::string& path, const TextureProperties& props);
		virtual bool Load() override;
		virtual void Unload() override;
		unsigned char* GetTextureData() const { return m_TextureData; }

		TextureProperties GetProperties() const { return m_Properties; }

	private:
		TextureProperties   m_Properties;
		unsigned char*		m_TextureData;
	};
}