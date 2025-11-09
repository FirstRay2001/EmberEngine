// PortalGameLayer.cpp
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#include <Ember.h>
#include "PortalGameLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

PortalGameLayer::PortalGameLayer() :
	Ember::Layer("PortalGameLayer"),
	m_Camera(Ember::CreateScope<Ember::Camera>(Ember::Camera(16.0f / 9.0f)))
{
	// Graphics Test
	m_VertexArray = Ember::Ref<Ember::VertexArray>(Ember::VertexArray::Create());
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	Ember::Ref<Ember::VertexBuffer> vertexBuffer(Ember::VertexBuffer::Create(vertices, sizeof(vertices)));
	Ember::BufferLayout layout = {
		{ Ember::ShaderDataType::Float3, "a_Position" },
		{ Ember::ShaderDataType::Float3, "a_Normal" },
		{ Ember::ShaderDataType::Float2, "a_TexCoord" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[] = {
		0,1,2,
		0,2,4,
		6,7,8,
		6,8,10,
		12,13,14,
		12,14,16,
		18,19,20,
		18,20,22,
		24,25,26,
		24,26,28,
		30,31,32,
		30,32,34 };
	Ember::Ref<Ember::IndexBuffer> indexBuffer(Ember::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_ShaderLibrary = Ember::CreateRef<Ember::ShaderLibrary>();
	auto shader = m_ShaderLibrary->Load("Asset/Shader/BlinnPhong.glsl");

	m_Texture = Ember::Texture2D::Create("Asset/Texture/GridBox_Default.png");

	// TODO: 创建材质
	m_Material = Ember::Material::Create("BoxMaterial");

	// 设置材质参数
	m_Material->SetAlbedoTexture(m_Texture);

	m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });
}

void PortalGameLayer::OnUpdate(const Ember::Timestep& timestep)
{
	float deltaSeconds = timestep.GetSeconds();
	float moveAmount = m_MoveSpeed * deltaSeconds;
	float rotateAmount = m_RotateSpeed * deltaSeconds;

	// 相机移动逻辑
	glm::vec3 forward = m_Camera->GetForwardDirection();
	glm::vec3 right = m_Camera->GetRightDirection();
	if (Ember::Input::IsKeyPressed(EMBER_KEY_A))
		m_Camera->SetPosition(m_Camera->GetPosition() + -1.0f * right * moveAmount);
	if (Ember::Input::IsKeyPressed(EMBER_KEY_D))
		m_Camera->SetPosition(m_Camera->GetPosition() + right * moveAmount);
	if (Ember::Input::IsKeyPressed(EMBER_KEY_W))
		m_Camera->SetPosition(m_Camera->GetPosition() + forward * moveAmount);
	if (Ember::Input::IsKeyPressed(EMBER_KEY_S))
		m_Camera->SetPosition(m_Camera->GetPosition() + -1.0f * forward * moveAmount);

	// 相机旋转逻辑
	glm::quat NewQuat = m_Camera->GetRotation();
	if (Ember::Input::IsKeyPressed(EMBER_KEY_Q))
	{
		glm::quat deltaRotation = glm::angleAxis(glm::radians(rotateAmount), glm::vec3(0.0f, 1.0f, 0.0f));
		NewQuat = deltaRotation * NewQuat;
		NewQuat = glm::normalize(NewQuat);
		m_Camera->SetRotation(NewQuat);
	}
	if (Ember::Input::IsKeyPressed(EMBER_KEY_E))
	{
		glm::quat deltaRotation = glm::angleAxis(glm::radians(-rotateAmount), glm::vec3(0.0f, 1.0f, 0.0f));
		NewQuat = deltaRotation * NewQuat;
		NewQuat = glm::normalize(NewQuat);
		m_Camera->SetRotation(NewQuat);
	}
	m_Camera->SetRotation(NewQuat);

	//////////////// 渲染流程 ////////////////////
	Ember::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.2f, 1.0f });
	Ember::RenderCommand::Clear();

	// 渲染场景
	Ember::Renderer::BeginScene(*m_Camera.get());
	{
		// 设置Shader颜色Uniform
		auto shader = m_ShaderLibrary->Get("BlinnPhong");

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));


		Ember::Renderer::Submit(shader, m_Material, m_VertexArray, transform);
	}
	Ember::Renderer::EndScene();
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
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_BoxColor));
	ImGui::End();
}
