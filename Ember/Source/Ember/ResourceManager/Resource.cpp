// Resource.h
// 资源类
// created by FirstRay2001, Nov/9/2001

#include "emberpch.h"
#include "Resource.h"
#include "Ember/Renderer/Shader.h"


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
}