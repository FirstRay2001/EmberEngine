// PortalGameLayer.cpp
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#include <Ember.h>
#include "PortalGameLayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

PortalGameLayer::PortalGameLayer() :
	Ember::Layer("PortalGameLayer"),
	m_Camera(Ember::CreateScope<Ember::Camera>(Ember::Camera(16.0f / 9.0f)))
{
}

void PortalGameLayer::OnAttach()
{
	
}

void PortalGameLayer::OnDetach()
{
}

void PortalGameLayer::OnUpdate(const Ember::Timestep& timestep)
{
	
}

void PortalGameLayer::OnEvent(Ember::Event& e)
{
	Ember::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Ember::WindowResizeEvent>([this](Ember::WindowResizeEvent& e)
		{
			m_Camera->SetScreentSize(e.GetWidth(), e.GetHeight());
			return false;
		});
}

void PortalGameLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_BoxColor));
	ImGui::End();
}
