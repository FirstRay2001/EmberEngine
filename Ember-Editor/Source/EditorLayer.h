// PortalGameLayer.h
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#pragma once

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
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		Scope<Camera> m_Camera;
		Ref<Texture2D> m_Texture;
		Ref<Material> m_Material;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec3 m_BoxColor = { 0.2f, 0.32f, 0.8f };

		float m_MoveSpeed = 1.0f;
		float m_RotateSpeed = 100.0f;

		bool m_FirstMouseMovement = true;
		glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };

		std::future<uint32_t> m_TextureLoadFuture;
	};
}