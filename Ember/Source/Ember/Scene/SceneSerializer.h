// SceneSerializer.h
// 场景序列化器
// created by FirstRay2001, Nov/20/2025

#pragma once

#include "Scene.h"

namespace YAML
{
	class Node;
}

namespace Ember
{
	class Entity;

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		// 序列化场景到文件
		void Serialize(const std::string& filepath);

		// 序列化运行时场景到文件
		void SerializeRuntime(const std::string& filepath);

		struct DeserializeParams
		{
			bool bUseTransform = true;
			bool bUseUUID = true;
		};

		// 反序列化实体
		void DeserializeEntity(const YAML::Node& entityNode, DeserializeParams params = {});

		// 从文件反序列化场景
		bool Deserialize(const std::string& filepath);

		// 从文件反序列化运行时场景
		bool DeserializeRuntime(const std::string& filepath);

		// 序列化预制体
		void SerializePrefab(const uint32_t entityID, const std::string& filepath);

		// 反序列化预制体
		bool DeserializePrefab(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}

