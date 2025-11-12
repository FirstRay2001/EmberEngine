// Scene.ch
// ECS场景
// created by FirstRay2001, Nov/12/2025

#pragma once

#include "entt.hpp"
#include "Ember/Core/Timestep.h"

namespace Ember
{
	class Entity;
	class VertexArray;
	class Material;

	class Scene
	{
		friend Entity;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity_Default_Name");

		void OnUpdate(const Timestep& timestep);

	private:
		entt::registry m_Registry;
	};
}
