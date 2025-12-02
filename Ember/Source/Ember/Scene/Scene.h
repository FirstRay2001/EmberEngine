// Scene.h
// ECS场景
// created by FirstRay2001, Nov/12/2025

#pragma once

#include "entt.hpp"
#include "Ember/Core/UUID.h"
#include "Ember/Core/Timestep.h"
#include "Ember/Renderer/EditorCamera.h"

namespace Ember
{
	class VertexArray;
	class Material;

	class Scene
	{
		friend class Entity;
		friend class SceneHierarchy;
		friend class SceneSerializer;
		friend class ScriptEngine;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity_Default_Name");
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "Entity_Default_Name");
		void DestroyEntity(Entity& entity);

		void OnStartRuntime();
		void OnStopRuntime();
		void OnUpdateRuntime(const Timestep& timestep);
		void OnUpdateEditor(const Timestep& timestep, EditorCamera& editorCamera, Entity selectedEntity);
		void OnViewportResize(uint32_t width, uint32_t height);

		static Ref<Scene> Copy(Ref<Scene> src);

	public:
		Entity GetPrimaryCameraEntity();
		Entity GetEntityByUUID(UUID uuid);

	private:
		void UpdateScripts(const Timestep& timestep);
		void RenderRuntime();
		void RenderEditor(EditorCamera& editorCamera, Entity selectedEntity);

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		std::unordered_map<UUID, Entity> m_EntityMap;
	};
}
