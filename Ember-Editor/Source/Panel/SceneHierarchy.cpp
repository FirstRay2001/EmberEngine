// SceneHierarchy.cpp
// 场景层级面板
// created by FirstRay2001, Nov/13/2025

#include "SceneHierarchy.h"
#include "imgui/imgui.h"
#include "Ember/Scene/Component.h"
#include "glm/gtc/type_ptr.hpp"

namespace Ember
{
	SceneHierarchy::SceneHierarchy(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchy::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchy::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// 遍历场景中的实体并绘制节点
		m_Context->m_Registry.view<TagComponent>().each([&](auto entityID, auto tag)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_SelectedEntity)
			DrawComponents(m_SelectedEntity);
		ImGui::End();
	}

	void SceneHierarchy::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_SelectedEntity = entity;
		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchy::DrawComponents(Entity entity)
	{
		// Tag Componnet
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			ImGui::Text("Tag"); ImGui::SameLine(0, 20);
			if (ImGui::InputText("", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		// Transform Component
		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 1.0f);
				ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);

				ImGui::TreePop();
			}
		}

		// Mesh Component
		if (entity.HasComponent<MeshComponent>())
		{
			ImGui::Text("Mesh Component");
			// 这里可以添加更多Mesh组件的属性编辑
		}
	}
}