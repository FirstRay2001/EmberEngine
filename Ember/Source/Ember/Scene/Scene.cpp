// Scene.cpp
// ECS场景
// created by FirstRay2001, Nov/12/2025

#include "emberpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "Ember/Renderer/Renderer.h"

namespace Ember
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::OnUpdate(const Timestep& timestep)
	{
		// 遍历所有带有MeshComponent和TransformComponent的实体并渲染它们
		auto view = m_Registry.view<TransformComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity).GetTransform();
			auto& meshComp = view.get<MeshComponent>(entity);
			auto mesh = meshComp.GetMesh();
			Renderer::Submit(mesh, transform);
		}
	}
}
