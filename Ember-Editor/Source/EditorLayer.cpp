// PortalGameLayer.cpp
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#include <Ember.h>
#include "EditorLayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Ember/Scene/Component.h"

namespace Ember
{
	EditorLayer::EditorLayer() :
		Layer("PortalGameLayer"),
		m_Camera(CreateScope<Camera>(Camera(16.0f / 9.0f)))
	{
	}

	void EditorLayer::OnAttach()
	{
		// Cube Mesh
		auto vertexArray = VertexArray::CreateCube(glm::vec3(5.0f, 5.0f, 1.0f));

		//// 异步加载Shader
		//ShaderLibrary::Get().LoadAsync("Asset/Shader/BlinnPhong.glsl");

		//// 异步加载纹理
		//TextureLibrary::Get().LoadAsync("Asset/Texture/GridBox_Default.png");

		// 同步加载Shader
		auto shader = ShaderLibrary::Get().LoadSync("Asset/Shader/BlinnPhong.glsl");

		// 同步加载纹理
		auto texture = TextureLibrary::Get().LoadSync("Asset/Texture/GridBox_Default.png");

		// 设置材质参数
		auto material = Material::Create("BoxMaterial");
		material->SetSpecularColor(glm::vec3(0.3f));
		material->SetAlbedoTexture(texture);

		m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });

		// 初始化Framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		// 设置活动场景
		m_ActiveScene = CreateRef<Scene>();

		// 初始化Entity
		m_BoxEntity = m_ActiveScene->CreateEntity("BoxEntity");
		m_BoxEntity.AddComponent<MeshComponent>(vertexArray, material, shader);
		auto& boxTransform = m_BoxEntity.GetComponent<TransformComponent>();
		boxTransform.Position = { 1.0f, 0.0f, -5.0f };
		boxTransform.Rotation = { 15.0f, 25.0f, 0.0f };
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(const Timestep& timestep)
	{
		float deltaSeconds = timestep.GetSeconds();
		float moveAmount = m_MoveSpeed * deltaSeconds;

		// 相机移动逻辑
		if(m_ViewportFocused)
		{
			glm::vec3 forward = m_Camera->GetForwardDirection();
			glm::vec3 right = m_Camera->GetRightDirection();
			if (Input::IsKeyPressed(EMBER_KEY_A))
				m_Camera->SetPosition(m_Camera->GetPosition() + -1.0f * right * moveAmount);
			if (Input::IsKeyPressed(EMBER_KEY_D))
				m_Camera->SetPosition(m_Camera->GetPosition() + right * moveAmount);
			if (Input::IsKeyPressed(EMBER_KEY_W))
				m_Camera->SetPosition(m_Camera->GetPosition() + forward * moveAmount);
			if (Input::IsKeyPressed(EMBER_KEY_S))
				m_Camera->SetPosition(m_Camera->GetPosition() + -1.0f * forward * moveAmount);

			// 相机旋转逻辑
			if (Input::IsMouseButtonPressed(EMBER_MOUSE_BUTTON_RIGHT))
			{
				if (m_FirstMouseMovement)
				{
					auto [x, y] = Input::GetMousePosition();
					m_LastMousePosition = { x, y };
					m_FirstMouseMovement = false;
				}
				else
				{
					// 计算鼠标偏移
					auto [x, y] = Input::GetMousePosition();
					float xOffset = x - m_LastMousePosition.x;
					float yOffset = y - m_LastMousePosition.y;
					m_LastMousePosition = { x, y };

					// 应用鼠标偏移到相机旋转
					xOffset *= m_CameraSensitivity;
					yOffset *= m_CameraSensitivity;
					glm::quat yaw = glm::angleAxis(glm::radians(-xOffset), glm::vec3(0.0f, 1.0f, 0.0f));
					glm::quat pitch = glm::angleAxis(glm::radians(-yOffset), right);
					glm::quat currentRotation = m_Camera->GetRotation();
					glm::quat newRotation = glm::normalize(pitch * yaw * currentRotation);
					m_Camera->SetRotation(newRotation);
				}
			}
			else
			{
				m_FirstMouseMovement = true;
			}
		}

		//// 非阻塞获取纹理
		//if (m_Texture == nullptr)
		//{
		//	m_Texture = TextureLibrary::Get().GetTextureAsync("GridBox_Default");
		//	if (m_Texture != nullptr)
		//		m_Material->SetAlbedoTexture(m_Texture);
		//}

		//// 非阻塞获取Shader
		//if (m_Shader == nullptr)
		//	m_Shader = ShaderLibrary::Get().GetShaderAsync("BlinnPhong");

		//////////////// 渲染流程 ////////////////////
		m_Framebuffer->Bind();
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.13f, 1.0f });
			RenderCommand::Clear();

			// 渲染场景
			Renderer::BeginScene(*m_Camera.get());
			{
				m_ActiveScene->OnUpdate(timestep);
			}
			Renderer::EndScene();
		}
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{

	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// Hierarchy面板
		ImGui::Begin("Hierarchy");
		ImGui::End();

		// Inspector面板
		ImGui::Begin("Inspector");
		const float dragSpeed = 0.01f;
		ImGui::DragFloat3("Location", glm::value_ptr(m_BoxEntity.GetComponent<TransformComponent>().Position), dragSpeed * 1.0f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(m_BoxEntity.GetComponent<TransformComponent>().Rotation), dragSpeed * 5.0f);
		ImGui::DragFloat3("Scale", glm::value_ptr(m_BoxEntity.GetComponent<TransformComponent>().Scale), dragSpeed * 0.2f);
		ImGui::End();

		// 渲染Framebuffer内容
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera->SetScreentSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)(uintptr_t)textureID, 
			ImVec2{ (float)m_Framebuffer->GetSpecification().Width, (float)m_Framebuffer->GetSpecification().Height }, 
			ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::End();
	}
}
