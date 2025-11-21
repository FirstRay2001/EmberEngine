// Scene.ch
// ECS场景
// created by FirstRay2001, Nov/12/2025

#pragma once

#include "entt.hpp"
#include "Ember/Core/Timestep.h"

namespace Ember
{
	class VertexArray;
	class Material;

	class Scene
	{
		friend class Entity;
		friend class SceneHierarchy;
		friend class SceneSerializer;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity_Default_Name");
		void DestroyEntity(Entity& entity);

		void OnUpdate(const Timestep& timestep);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		void UpdateScripts(const Timestep& timestep);
		void Render();

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};
}
