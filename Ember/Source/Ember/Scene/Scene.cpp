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
		// 更新脚本组件
		UpdateScripts(timestep);

		// 渲染
		Render();
	}

	void Scene::UpdateScripts(const Timestep& timestep)
	{
		auto view = m_Registry.view<NativeScriptComponent>();
		for (auto entity : view)
		{
			auto& scriptComp = view.get<NativeScriptComponent>(entity);

			// 如果脚本实例不存在，则实例化脚本并调用OnCreate方法
			if (!scriptComp.Instance)
			{
				scriptComp.Instance = scriptComp.InstantiateScript();
				scriptComp.Instance->m_Entity = Entity{ entity, this };
				scriptComp.Instance->OnCreate();
			}

			// 调用脚本的OnUpdate方法
			scriptComp.Instance->OnUpdate(timestep);
		}
	}

	void Scene::Render()
	{
		// 清屏
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.13f, 1.0f });
		RenderCommand::Clear();

		// 获取光照组件
		{
			// 点光源
			auto pointLightView = m_Registry.view<PointLightComponent>();
			for (auto entity : pointLightView)
			{
				auto& pointLightComp = pointLightView.get<PointLightComponent>(entity);

				// 把Transform组件同步到PointLight
				auto& transform = m_Registry.get<TransformComponent>(entity);
				pointLightComp.m_PointLight.Position = transform.Position;

				Renderer::AddPointLight(pointLightComp.m_PointLight);
			}

			// 平行光
			auto dirLightView = m_Registry.view<DirectionalLightComponent>();
			for (auto entity : dirLightView)
			{
				auto& dirLightComp = dirLightView.get<DirectionalLightComponent>(entity);
				
				// 把Transform组件同步到DirectionalLight
				auto& transform = m_Registry.get<TransformComponent>(entity);
				dirLightComp.m_DirectionalLight.Direction = glm::normalize(transform.GetTransform() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

				Renderer::AddDirectionalLight(dirLightComp.m_DirectionalLight);
			}
		}

		// 获取主相机实体
		Camera camera;
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camComp = view.get<CameraComponent>(entity);
				camera = camComp.m_Camera;

				// 把Transform组件同步到Camera
				auto& transform = m_Registry.get<TransformComponent>(entity);
				camera.SetPosition(transform.Position);
				camera.SetRotation(glm::quat(glm::radians(transform.Rotation)));
				break; // 只取第一个相机
			}
		}

		// 传入相机数据
		Renderer::BeginScene(camera);

		// 遍历所有带有MeshComponent和TransformComponent的实体并渲染它们
		auto view = m_Registry.view<TransformComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity).GetTransform();
			auto& meshComp = view.get<MeshComponent>(entity);
			auto& mesh = meshComp.GetMesh();
			Renderer::Submit(mesh, transform);
		}

		Renderer::EndScene();
	}
}
