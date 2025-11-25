// PortalGameLayer.cpp
// 游戏实现层
// created by FirstRay2001, Nov/9/2025

#include <Ember.h>
#include "EditorLayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <ImGuizmo.h>

#include "Ember/Scene/Component.h"
#include "Ember/Scene/SceneSerializer.h"
#include "Script/CameraController.h"
#include "Ember/Utils/PlatformUtils.h"
#include "Ember/Math/Math.h"

namespace Ember
{
	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer() :
		Layer("PortalGameLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		// 初始化Framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			FramebufferTextureSpecification(FramebufferTextureFormat::RGBA8),
			FramebufferTextureSpecification(FramebufferTextureFormat::RED_INTEGER),
			FramebufferTextureSpecification(FramebufferTextureFormat::Depth)
		};
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		// 设置活动场景
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize(fbSpec.Width, fbSpec.Height);
#if 0
		// 同步加载Shader
		auto shader = ShaderLibrary::Get().LoadSync("Asset/Shader/BlinnPhong.glsl");

		// 同步加载纹理
		auto texture = TextureLibrary::Get().LoadSync("Asset/Texture/GridBox_Default.png");

		// Box实体
		auto gridMaterial = Material::Create("BoxMaterial");
		gridMaterial->SetSpecularColor(glm::vec3(0.2f));
		gridMaterial->SetAlbedoTexture(texture);
		m_BoxEntity = m_ActiveScene->CreateEntity("Wall");
		m_BoxEntity.AddComponent<MeshComponent>(VertexArray::CreateCube(glm::vec3(5.0f, 5.0f, 1.0f)), gridMaterial, shader);
		auto& boxTransform = m_BoxEntity.GetComponent<TransformComponent>();
		boxTransform.Position = { 1.0f, 0.0f, -5.0f };
		boxTransform.Rotation = { 0.0f, 25.0f, 0.0f };

		// Sphere实体
		auto sphereMaterial = Material::Create("BallMateial");
		sphereMaterial->SetAlbedo(glm::vec3(0.8f, 0.2f, 0.2f));
		sphereMaterial->SetSpecularColor(glm::vec3(0.8f));
		sphereMaterial->SetShininess(64.0f);
		m_SphereEntity = m_ActiveScene->CreateEntity("Ball");
		m_SphereEntity.AddComponent<MeshComponent>(VertexArray::CreateSphere(1.0f, 32, 32), sphereMaterial, shader);
		auto& sphereTransform = m_SphereEntity.GetComponent<TransformComponent>();
		sphereTransform.Position = { 2.0f, -2.0f, -2.0f };
		sphereTransform.Rotation = { 0.0f, -15.0f, 0.0f };

		// 场景相机
		auto camera = Camera(16.0f / 9.0f, 45.0f, 0.1f, 100.0f);
		m_EditorCamera = m_ActiveScene->CreateEntity("EditorCamera");
		m_EditorCamera.AddComponent<CameraComponent>(camera);
		m_EditorCamera.GetComponent<TransformComponent>().Position = { 0.0f, 0.0f, 5.0f };
		m_EditorCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		// 平行光实体
		m_DirectionalLight = m_ActiveScene->CreateEntity("DirectionalLight");
		auto& dirLight = m_DirectionalLight.AddComponent<DirectionalLightComponent>().m_DirectionalLight;
		dirLight.Ambient = glm::vec3(0.2f);
		dirLight.Diffuse = glm::vec3(0.5f);
		dirLight.Specular = glm::vec3(1.0f);
		m_DirectionalLight.GetComponent<TransformComponent>().Rotation = glm::vec3(-20.0f, 260.0f, 0.0f);

		// 点光源Mesh
		auto pointLightVAO = VertexArray::CreateSphere(0.1f, 18, 18);

		// 点光源材质
		auto pointLightMaterial = Material::Create("PointLightMaterial");
		pointLightMaterial->SetAlbedo(glm::vec3(1.0));

		// 点光源Shader
		auto pointLightShader = ShaderLibrary::Get().LoadSync("Asset/Shader/SimpleColor.glsl");

		// 点光源实体
		m_PointLight = m_ActiveScene->CreateEntity("PointLight");
		m_PointLight.AddComponent<MeshComponent>(pointLightVAO, pointLightMaterial, pointLightShader);
		auto& pointLight = m_PointLight.AddComponent<PointLightComponent>().m_PointLight;
		pointLight.Ambient = glm::vec3(0.1f);
		pointLight.Diffuse = glm::vec3(0.5f);
		pointLight.Specular = glm::vec3(1.0f);
		m_PointLight.GetComponent<TransformComponent>().Position = glm::vec3(2.0f, 0.5f, -2.0f);

		// Grid实体
		auto gridShader = ShaderLibrary::Get().LoadSync("Asset/Shader/SimpleColor.glsl");
		auto gridVAO = VertexArray::CreateGrid(10.0f, 10);
		auto gridEntity = m_ActiveScene->CreateEntity("Grid");
		gridEntity.AddComponent<GridComponent>(gridVAO, gridShader);
#endif
		
		// 初始化场景相机
		m_EditorCamera = CreateRef<EditorCamera>(45.0f, 1.778f, 0.1f, 1000.0f);
		m_EditorCamera->SetScreenSize(fbSpec.Width, fbSpec.Height);

		// 初始化Hierarchy面板
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(const Timestep& timestep)
	{
		// 更新Framebuffer规格
		if (m_ViewportSize.x != m_Framebuffer->GetSpecification().Width || m_ViewportSize.y != m_Framebuffer->GetSpecification().Height)
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera->SetScreenSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// 更新编辑器相机
		if(m_ViewportFocused)
			m_EditorCamera->OnUpdate(timestep);

		//////////////// 更新Scene ////////////////////
		m_Framebuffer->Bind();
		{
			// 清屏
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.13f, 1.0f });
			RenderCommand::Clear();

			// 清除颜色附件
			m_Framebuffer->ClearAttachment(1, -1);

			// m_ActiveScene->OnUpdateRuntime(timestep);
			m_ActiveScene->OnUpdateEditor(timestep, *m_EditorCamera);

			// 鼠标拾取
			if (m_ViewportHovered && !ImGuizmo::IsUsing() && Input::IsMouseButtonPressed(EMBER_MOUSE_BUTTON_LEFT) && !Input::IsKeyPressed(EMBER_KEY_LEFT_ALT))
			{
				auto [mx, my] = ImGui::GetMousePos();
				mx -= m_ViewportBounds[0].x;
				my -= m_ViewportBounds[0].y;
				my = m_ViewportSize.y - my;
				glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
				if (mx >= 0 && my >= 0 && mx < viewportSize.x && my < viewportSize.y)
				{
					int pixelData = m_Framebuffer->ReadPixel(1, (int)mx, (int)my);
					if (pixelData != -1)
					{
						Entity selectedEntity((entt::entity)pixelData, m_ActiveScene.get());
						m_SceneHierarchyPanel.SetSelectedEntity(selectedEntity);
					}
				}
			}
		}
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera->OnEvent(e);
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

		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 450.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
					NewScene();

				if (ImGui::MenuItem("Open..."))
					LoadScene();

				if (ImGui::MenuItem("Save As..."))
					SaveScene();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();

				ImGui::EndMenu();
			}

			

			ImGui::EndMenuBar();
		}

		// ImGui Demo
		// ImGui::ShowDemoWindow();

		// Hierarchy面板
		m_SceneHierarchyPanel.OnImGuiRender();

		// ContentBrowser面板
		m_ContentBrowserPanel.OnImGuiRender();

		// Viewport面板
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		auto viewportOffset = ImGui::GetCursorPos();
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
		ImGui::Image((void*)(uintptr_t)textureID, 
			ImVec2{ (float)m_Framebuffer->GetSpecification().Width, (float)m_Framebuffer->GetSpecification().Height }, 
			ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;
		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };
		// 拖拽加载
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				LoadScene(std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}


		// Gizmo操作面板
		static ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
		ImVec2 windowPos = ImVec2(io.DisplaySize.x - 260.0f, 10.0f);
		ImGui::SetNextWindowPos(windowPos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(250, 0), ImGuiCond_FirstUseEver);
		ImGui::Begin("Gizmo Operation", nullptr,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::RadioButton("Translate", gizmoOperation == ImGuizmo::TRANSLATE))
			gizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", gizmoOperation == ImGuizmo::ROTATE))
			gizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", gizmoOperation == ImGuizmo::SCALE))
			gizmoOperation = ImGuizmo::SCALE;
		ImGui::End();

		// Gizmo
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// 获取相机矩阵
			const glm::mat4& cameraProjection = m_EditorCamera->GetProjectionMatrix();
			glm::mat4 cameraView = m_EditorCamera->GetViewMatrix();

			// 获取实体矩阵
			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 entityTransform = transformComponent.GetTransform();

			// 使用ImGuizmo
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				gizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(entityTransform));

			// 如果被修改，更新Transform组件
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(entityTransform, translation, rotation, scale);
				transformComponent.Position = translation;
				transformComponent.Rotation = rotation;
				transformComponent.Scale = scale;
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::SaveScene()
	{
		std::string filepath = FileDialog::SaveFile("Ember Scene (*.ember)\0*.ember\0");

		// 用户取消
		if (filepath.empty())
			return;

		SceneSerializer serilizer(m_ActiveScene);
		serilizer.Serialize(filepath);
	}

	void EditorLayer::LoadScene()
	{
		std::string filepath = FileDialog::OpenFile("Ember Scene (*.ember)\0*.ember\0");

		// 用户取消
		if (filepath.empty())
			return;

		LoadScene(std::filesystem::path(filepath));
	}

	void EditorLayer::LoadScene(const std::filesystem::path& path)
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serilizer(m_ActiveScene);
		serilizer.Deserialize(path.string());
	}
}
