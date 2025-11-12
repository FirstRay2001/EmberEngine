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
		Ember::Layer("PortalGameLayer"),
		m_Camera(Ember::CreateScope<Ember::Camera>(Ember::Camera(16.0f / 9.0f)))
	{
	}

	void EditorLayer::OnAttach()
	{
		// Graphics Test
		auto vertexArray = Ember::Ref<Ember::VertexArray>(Ember::VertexArray::Create());
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
		vertexArray->AddVertexBuffer(vertexBuffer);

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
		vertexArray->SetIndexBuffer(indexBuffer);

		//// 异步加载Shader
		//ShaderLibrary::Get().LoadAsync("Asset/Shader/BlinnPhong.glsl");

		//// 异步加载纹理
		//TextureLibrary::Get().LoadAsync("Asset/Texture/GridBox_Default.png");

		// 同步加载Shader
		auto shader = Ember::ShaderLibrary::Get().LoadSync("Asset/Shader/BlinnPhong.glsl");

		// 同步加载纹理
		auto texture = Ember::TextureLibrary::Get().LoadSync("Asset/Texture/GridBox_Default.png");

		// 设置材质参数
		auto material = Ember::Material::Create("BoxMaterial");
		material->SetSpecularColor(glm::vec3(0.3f));
		material->SetAlbedoTexture(texture);

		m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });

		// 初始化Framebuffer
		Ember::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Ember::Framebuffer::Create(fbSpec);

		// 设置活动场景
		m_ActiveScene = CreateRef<Scene>();

		// 初始化Entity
		m_BoxEntity = m_ActiveScene->CreateEntity("BoxEntity");
		m_BoxEntity.AddComponent<MeshComponent>(vertexArray, material, shader);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(const Ember::Timestep& timestep)
	{
		float deltaSeconds = timestep.GetSeconds();
		float moveAmount = m_MoveSpeed * deltaSeconds;

		// 相机移动逻辑
		if(m_ViewportFocused)
		{
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
			if (Ember::Input::IsMouseButtonPressed(EMBER_MOUSE_BUTTON_RIGHT))
			{
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

		// 控制方块Transform
		{
			glm::mat4 newTransform = glm::mat4(1.0f);
			newTransform = glm::rotate(newTransform, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			newTransform = glm::translate(newTransform, glm::vec3(0.0f, -0.3f, 0.0f));
			auto& transform = m_BoxEntity.GetComponent<TransformComponent>();
			transform = newTransform;
		}

		//////////////// 渲染流程 ////////////////////
		m_Framebuffer->Bind();
		{
			Ember::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.13f, 1.0f });
			Ember::RenderCommand::Clear();

			// 渲染场景
			Ember::Renderer::BeginScene(*m_Camera.get());
			{
				m_ActiveScene->OnUpdate(timestep);
			}
			Ember::Renderer::EndScene();
		}
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Ember::Event& e)
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
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Ember::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_BoxColor));
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
