// PortalGameLayer.h
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#pragma once

#include "Ember/Scene/Scene.h"
#include "Ember/Scene/Entity.h"

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

		Scope<Camera> m_Camera;
		Ref<Texture2D> m_Texture;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec3 m_BoxColor = { 0.2f, 0.32f, 0.8f };

		float m_MoveSpeed = 1.0f;
		float m_CameraSensitivity = 0.2f;

		bool m_FirstMouseMovement = true;
		glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };

		glm::vec2 m_ViewportSize = { 0, 0 };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	};
}