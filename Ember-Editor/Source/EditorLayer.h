// PortalGameLayer.h
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#pragma once

#include "Ember/Scene/Scene.h"
#include "Ember/Scene/Entity.h"
#include "Ember/Renderer/EditorCamera.h"
#include "Panel/SceneHierarchy.h"
#include "Panel/ContentBrowserPanel.h"

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
		void NewScene();
		void SaveScene();
		void LoadScene();
		void LoadScene(const std::filesystem::path& path);
		void LoadPrefab(const std::filesystem::path& path);
		void ProcessDrag(const std::filesystem::path& path);

		// 设置面板上下文
		void SetPanelsContext();

	private:
		Ref<Scene> m_ActiveScene;
		Ref<EditorCamera> m_EditorCamera;
		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = { 1280, 720 };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		SceneHierarchy m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

#if 0
		Entity m_BoxEntity;
		Entity m_SphereEntity;
		Entity m_EditorCamera;
		Entity m_GameCamera;
		Entity m_DirectionalLight;
		Entity m_PointLight;
#endif
	};
}