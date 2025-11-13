// PortalGameLayer.h
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#pragma once

#include "Ember/Scene/Scene.h"
#include "Ember/Scene/Entity.h"
#include "Panel/SceneHierarchy.h"

namespace Ember
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(const Timestep& timestep) override;

		virtual void OnEvent(Event& e) override;

		virtual void OnImGuiRender() override;

	private:
		Ref<Scene> m_ActiveScene;
		Entity m_BoxEntity;
		Entity m_EditorCamera;
		Entity m_GameCamera;

		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = { 0, 0 };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		SceneHierarchy m_SceneHierarchyPanel;
	};
}