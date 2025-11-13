// SceneHierarchy.cpp
// 场景层级面板
// created by FirstRay2001, Nov/13/2025

#include "SceneHierarchy.h"
#include "imgui/imgui.h"
#include "Ember/Scene/Component.h"

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
}