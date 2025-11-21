// SceneSerializer.h
// 场景序列化器
// created by FirstRay2001, Nov/20/2025

#pragma once

#include "Scene.h"

namespace Ember
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		// 序列化场景到文件
		void Serialize(const std::string& filepath);

		// 序列化运行时场景到文件
		void SerializeRuntime(const std::string& filepath);

		// 从文件反序列化场景
		bool Deserialize(const std::string& filepath);

		// 从文件反序列化运行时场景
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}

