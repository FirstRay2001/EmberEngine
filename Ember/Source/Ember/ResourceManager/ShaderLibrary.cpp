// ShaderLibrary.cpp
// 着色器管理器
// created by FirstRay2001, Nov/11/2025

#include "emberpch.h"
#include "ShaderLibrary.h"
#include "Ember/Renderer/Shader.h"

namespace Ember
{
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		if (Exists(name))
			EMBER_CORE_WARN("Shader already exists! Overwriting {0}", name.c_str());
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::LoadSync(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::LoadSync(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(shader);
		return shader;
	}

    void ShaderLibrary::LoadAsync(const std::string& filepath)
    {
       auto shaderFuture = ResourceManager::Get().LoadShaderAsync(filepath, ShaderProperties(), [this](ResourceHandle handle)
           {
               EMBER_CORE_TRACE("Shader loaded with handle: {}", handle);
           });

       auto name = Shader::FilePathToName(filepath);

	   // Note - 很关键的知识点
	   //	   - 首先，future不能拷贝，只能移动，所以使用move
	   //	   - 其次，使用insert + make_pair的方式避免unordered_map构造时的拷贝
       m_LoadingShaders.insert(std::make_pair(name, std::move(shaderFuture)));
    }

    Ref<Shader> ShaderLibrary::GetShaderSync(const std::string& name)
    {
        if (Exists(name))
            return m_Shaders[name];

		// 如果Shader正在加载，等待其完成
		auto it = m_LoadingShaders.find(name);
		if (it != m_LoadingShaders.end())
		{
			auto handle = it->second.get(); // 阻塞等待加载完成
			Ref<IResource> res = ResourceManager::Get().GetResource(handle);
			Ref<Shader> shader = std::dynamic_pointer_cast<ShaderResource>(res)->GetShader();
			Add(shader);
			m_LoadingShaders.erase(it);
			return m_Shaders[name];
		}
		else
		{
			EMBER_CORE_ERROR("Shader not found! {0}", name.c_str());
		}
		return nullptr;
    }


    Ref<Shader> ShaderLibrary::GetShaderAsync(const std::string& name)
    {
       if (Exists(name))
           return m_Shaders[name];

       // 检查该Shader是否加载中
       auto it = m_LoadingShaders.find(name);
       if (it != m_LoadingShaders.end())
       {
           // 非阻塞获取Shader
		   auto status = it->second.wait_for(std::chrono::seconds(0));
           if (status == std::future_status::ready)
           {
               ResourceHandle handle = it->second.get();
               Ref<IResource> res = ResourceManager::Get().GetResource(handle);
               Ref<Shader> shader = std::dynamic_pointer_cast<ShaderResource>(res)->GetShader();
               Add(shader);
               m_LoadingShaders.erase(it);
               return m_Shaders[name];
           }
		   else
		   {
			   EMBER_CORE_WARN("Shader {0} is still loading.", name.c_str());
		   }
       }
       else
       {
           EMBER_CORE_ERROR("Shader not found! {0}", name.c_str());
       }

       return nullptr;
    }

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}