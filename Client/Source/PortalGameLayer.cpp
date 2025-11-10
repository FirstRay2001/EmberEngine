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

	// 异步加载Shader
	Ember::ResourceManager::Get().LoadShaderAsync("Asset/Shader/BlinnPhong.glsl", {},
		[this](Ember::ResourceHandle handle)
		{
			Ember::Ref<Ember::IResource> res = Ember::ResourceManager::Get().GetResource(handle);
			m_Shader = std::dynamic_pointer_cast<Ember::ShaderResource>(res)->GetShader();
		});

	m_Material = Ember::Material::Create("BoxMaterial");

	// 异步加载纹理
	m_TextureLoadFuture = Ember::ResourceManager::Get().LoadTextureAsync("Asset/Texture/GridBox_Default.png", {});

	// 设置材质参数
	m_Material->SetSpecularColor(glm::vec3(0.3f));

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
	if (m_FirstMouseMovement)
	{
		auto [x, y] = Ember::Input::GetMousePosition();
		m_LastMousePosition = { x, y };
		m_FirstMouseMovement = false;
	}
	else
	{
		// 计算鼠标偏移
		auto [x, y] = Ember::Input::GetMousePosition();
		float xOffset = x - m_LastMousePosition.x;
		float yOffset = y - m_LastMousePosition.y;
		m_LastMousePosition = { x, y };

		// 应用鼠标偏移到相机旋转
		float sensitivity = 0.1f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;
		glm::quat yaw = glm::angleAxis(glm::radians(-xOffset), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat pitch = glm::angleAxis(glm::radians(-yOffset), right);
		glm::quat currentRotation = m_Camera->GetRotation();
		glm::quat newRotation = glm::normalize(pitch * yaw * currentRotation);
		m_Camera->SetRotation(newRotation);
	}

	// 检查异步纹理加载是否完成
	// Note - 不在子线程中创建GPU纹理, 仅在子线程中将数据加载到内存
	//		- 在主线程中创建GPU纹理
	//		- 如果不这样做，会出现奇怪的纹理创建异常
	if (m_TextureLoadFuture.valid() &&
		m_TextureLoadFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
	{
		Ember::ResourceHandle handle = m_TextureLoadFuture.get();
		Ember::Ref<Ember::IResource> res = Ember::ResourceManager::Get().GetResource(handle);
		auto* data = std::dynamic_pointer_cast<Ember::TextureResource>(res)->GetTextureData();
		auto prop = std::dynamic_pointer_cast<Ember::TextureResource>(res)->GetProperties();
		m_Texture = Ember::Texture2D::Create(data, prop.m_Width, prop.m_Height, prop.m_Channels);
		m_Material->SetAlbedoTexture(m_Texture);
	}

	//////////////// 渲染流程 ////////////////////
	Ember::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.13f, 1.0f });
	Ember::RenderCommand::Clear();

	// 渲染场景
	Ember::Renderer::BeginScene(*m_Camera.get());
	{
		// 判断异步资源有效性
		if (m_Shader && m_Texture)
		{
			glm::mat4 transform = glm::mat4(1.0f);
			transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::translate(transform, glm::vec3(0.0f, -0.3f, 0.0f));
			Ember::Renderer::Submit(m_Shader, m_Material, m_VertexArray, transform);
		}
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
