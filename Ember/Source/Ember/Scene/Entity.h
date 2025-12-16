// Entity.h
// ECS实体
// created by FirstRay2001, Nov/12/2025

#pragma once

#include "Component.h"
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
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		// 添加或更新
		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
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
		operator bool() const 
		{ 
			return m_EntityHandle != entt::null && m_Scene != nullptr && m_Scene->m_Registry.valid(m_EntityHandle);
		}

		// 获取实体ID
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		// 获取UUID
		UUID GetUUID() { return GetComponent<IDComponent>().m_ID; }

		// 无效化
		void Invalidate() { m_EntityHandle = entt::null; m_Scene = nullptr; }

		// 转换为entt::entity
		operator entt::entity() const { return m_EntityHandle; }

		// 比较运算符
		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
