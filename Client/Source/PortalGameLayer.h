// PortalGameLayer.h
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#pragma once

class PortalGameLayer : public Ember::Layer
{
public:
	PortalGameLayer();

	virtual void OnUpdate(const Ember::Timestep& timestep) override;

	virtual void OnEvent(Ember::Event& e) override;

	virtual void OnImGuiRender() override;

private:
	Ember::Ref<Ember::VertexArray> m_VertexArray;
	Ember::Ref<Ember::Shader> m_Shader;
	Ember::Scope<Ember::Camera> m_Camera;
	Ember::Ref<Ember::Texture2D> m_Texture;
	Ember::Ref<Ember::Material> m_Material;
	glm::vec3 m_BoxColor = { 0.2f, 0.32f, 0.8f };

	float m_MoveSpeed = 1.0f;
	float m_RotateSpeed = 100.0f;

	bool m_FirstMouseMovement = true;
	glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };

	std::future<uint32_t> m_TextureLoadFuture;
};

