// Entity.h
// ECS实体
// created by FirstRay2001, Nov/12/2025

#pragma once

#include "Scene.h"

namespace Ember
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;
		~Entity() = default;

		// 添加组件
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EMBER_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		// 获取组件
		template<typename T>
		T& GetComponent()
		{
			EMBER_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		//  获取组件（常量版本）
		template<typename T>
		const T& GetComponent() const
		{
			EMBER_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		// 检查是否有组件
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		// 移除组件
		template<typename T>
		void RemoveComponent()
		{
			EMBER_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		// 检测有效性
		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
