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
		// 获取主相机实体
		Camera camera;
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camComp = view.get<CameraComponent>(entity);
				camera = camComp.Camera;
				break; // 只取第一个相机
			}
		}

		//////////////// 渲染 ////////////////
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.13f, 1.0f });
		RenderCommand::Clear();

		Renderer::BeginScene(camera);

		// 遍历所有带有MeshComponent和TransformComponent的实体并渲染它们
		auto view = m_Registry.view<TransformComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity).GetTransform();
			auto& meshComp = view.get<MeshComponent>(entity);
			auto mesh = meshComp.GetMesh();
			Renderer::Submit(mesh, transform);
		}

		Renderer::EndScene();
	}
}
