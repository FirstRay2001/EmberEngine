// SceneHierarchy.cpp
// 场景层级面板
// created by FirstRay2001, Nov/13/2025

#include <filesystem>

#include "SceneHierarchy.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Ember/Scene/Component.h"
#include "glm/gtc/type_ptr.hpp"
#include "Ember/Utils/PlatformUtils.h"
#include "Ember/Renderer/Texture.h"
#include "Ember/Scripting/ScriptEngine.h"

namespace Ember
{
	extern const std::filesystem::path g_AssetPath;

	SceneHierarchy::SceneHierarchy(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchy::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectedEntity = {};
	}

	void SceneHierarchy::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// 遍历场景中的实体并绘制节点
		m_Context->m_Registry.view<TagComponent>().each([&](auto entityID, auto tag)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		// 左键点击空白处取消选中实体
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		// 创建实体的右键菜单
		if (ImGui::BeginPopupContextWindow(nullptr, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);
		}
		ImGui::End();
	}

	void SceneHierarchy::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		// 选中实体
		if (ImGui::IsItemClicked())
			m_SelectedEntity = entity;

		// 拖拽
		if (ImGui::BeginDragDropSource())
		{
			uint32_t data = entity;
			ImGui::SetDragDropPayload("HIERARCHY_ENTITY", (void*)&data, sizeof(uint32_t), ImGuiCond_Once);
			ImGui::Text("%s", tag.c_str());
			ImGui::EndDragDropSource();
		}

		// 展开节点
		if (opened)
		{
			// TODO: 绘制子实体节点
			ImGui::TreePop();
		}

		bool eneityDeleted = false;

		// 删除实体的右键菜单
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				eneityDeleted = true;
			ImGui::EndPopup();
		}

		if (eneityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectedEntity == entity)
				m_SelectedEntity = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float dragPower = 0.1f, float columWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// X标签
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{

		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		// X分量
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, dragPower, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y标签
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{

		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		// Y分量
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, dragPower, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z标签
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{

		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		// Z分量
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, dragPower, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	static void DrawColorPickControl(const std::string& label, glm::vec3& values, float columWidth = 100.0f)
	{
		// ImGuiIO& io = ImGui::GetIO();
		// auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::ColorEdit3("##Color", glm::value_ptr(values));
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	static bool DrawFloatControl(const std::string& label, float& value, float dragPower = 0.1f, float minValue = 0.0f, float maxValue = 0.0f)
	{
		bool changed = false;

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.f);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		changed = ImGui::DragFloat("##Value", &value, dragPower, minValue, maxValue, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	template<typename T, typename UIFunction>
	static void DrawSingleComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed 
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 3 });
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2);
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4{ 0.1f, 0.155f, 0.15f, 1.0f });
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor();

			// 避开滚动条
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;
				ImGui::EndPopup();
			}
			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchy::DrawComponents(Entity entity)
	{
		// Tag Componnet
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		// 添加组件按钮
		ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 125.0f);
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent"))
		{
			// 摄像机组件
			DisplayAddComponentEntry<CameraComponent>("Camera");

			// 模型组件
			ImGui::Separator();
			DisplayAddComponentEntry<ModelComponent>("Model Renderer");

			// 光源组件
			ImGui::Separator();
			DisplayAddComponentEntry<PointLightComponent>("Point Light");
			DisplayAddComponentEntry<DirectionalLightComponent>("Directional Light");

			// 脚本组件
			ImGui::Separator();
			DisplayAddComponentEntry<ScriptComponent>("Script");

			// 天空盒
			ImGui::Separator();
			DisplayAddComponentEntry<SkyboxComponent>("Skybox");

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		// Transform Component
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.8f, 0.3f, 0.2f, 1.0f });
		DrawSingleComponent<TransformComponent>("Transform", entity, [](auto& transform)
		{
			DrawVec3Control("Position", transform.Position);
			DrawVec3Control("Rotation", transform.Rotation);
			DrawVec3Control("Scale", transform.Scale, 0.02f);
		});
		ImGui::PopStyleColor();

		// Light Component
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.9f, 0.85f, 0.9f, 1.0f });
		{
			DrawSingleComponent<PointLightComponent>("Point Light", entity, [](auto& pointLightComp)
				{
					auto& pointLight = pointLightComp.m_PointLight;
					DrawColorPickControl("Ambient", pointLight.Ambient);
					DrawColorPickControl("Diffuse", pointLight.Diffuse);
					DrawColorPickControl("Specular", pointLight.Specular);
				});

			DrawSingleComponent<DirectionalLightComponent>("Directional Light", entity, [](auto& dirLightComp)
				{
					auto& dirLight = dirLightComp.m_DirectionalLight;
					DrawColorPickControl("Ambient", dirLight.Ambient);
					DrawColorPickControl("Diffuse", dirLight.Diffuse);
					DrawColorPickControl("Specular", dirLight.Specular);
				});
		}
		ImGui::PopStyleColor();

		// Camera Component
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.0f, 0.0f, 0.2f, 1.0f });
		DrawSingleComponent<CameraComponent>("Camera", entity, [](auto& cameraComp)
		{
			auto& camera = cameraComp.m_Camera;
			float fov = camera.GetFov();
			if (DrawFloatControl("Fov", fov, 1.0f, 1.0f, 120.0f))
			{
				camera.SetFov(fov);
			}

			float aspect = camera.GetAspectRatio();
			if (DrawFloatControl("Aspect Ratio", aspect, 0.01f, 0.1f, 4.0f))
			{
				camera.SetAspectRatio(aspect);
			}

			float nearClip = camera.GetNearClip();
			if (DrawFloatControl("Near Clip", nearClip, 0.1f, 0.1f, 100.0f))
			{
				camera.SetNearClip(nearClip);
			}

			float farClip = camera.GetFarClip();
			if (DrawFloatControl("Far Clip", farClip, 1.0f, 1.0f, 1000.0f))
			{
				camera.SetFarClip(farClip);
			}
		});
		ImGui::PopStyleColor();

		// Mesh Component
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.8f, 0.8f, 0.2f, 1.0f });
		DrawSingleComponent<MeshComponent>("Mesh Renderer", entity, [](auto& meshComp)
		{
				VertexArray::PrimitiveType primitiveType = meshComp.GetMesh().GetVertexArray()->GetPrimitiveType();
				std::string primitiveTypeName;
				switch (primitiveType)
				{
				case VertexArray::PrimitiveType::Cube:
					primitiveTypeName = "Cube";
					break;
				case VertexArray::PrimitiveType::Sphere:
					primitiveTypeName = "Sphere";
					break;
				default:
					primitiveTypeName = "None";
					break;
				};

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);

				ImGui::Text("Type");
				ImGui::NextColumn();
				ImGui::Text("%s", primitiveTypeName.c_str());

				// 显示不同类型的参数
				if (primitiveType == VertexArray::PrimitiveType::Cube)
				{
					auto scale = meshComp.GetMesh().GetVertexArray()->GetCubeScale();
					ImGui::NextColumn();
					ImGui::Text("Scale");
					ImGui::NextColumn();
					ImGui::Text("X: %.2f  Y: %.2f  Z: %.2f", scale.x, scale.y, scale.z);
				}
				else if (primitiveType == VertexArray::PrimitiveType::Sphere)
				{
					auto radius = meshComp.GetMesh().GetVertexArray()->GetSphereRadius();
					auto sectorCount = meshComp.GetMesh().GetVertexArray()->GetSphereSectorCount();
					auto stackCount = meshComp.GetMesh().GetVertexArray()->GetSphereStackCount();
					ImGui::NextColumn();
					ImGui::Text("Radius");
					ImGui::NextColumn();
					ImGui::Text("%.2f", radius);
				}
				else
				{
					ImGui::NextColumn();
					ImGui::Text("Model Mesh");
					ImGui::NextColumn();
				}

				ImGui::Separator();

				// Shader
				auto shader = meshComp.GetMesh().GetShader();
				std::string shaderName = shader ? shader->GetName() : "None";
				
				ImGui::NextColumn();
				ImGui::Text("Shader");
				ImGui::NextColumn();
				ImGui::Button(shaderName.c_str(), ImVec2{100.0f, 0.0f});

				// 拖拽添加Shader
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path shaderPath = std::filesystem::path(g_AssetPath) / path;

						// 检查扩展名
						if (shaderPath.extension() == ".glsl")
						{
							auto shader = ShaderLibrary::Get().LoadSync(shaderPath.string());
							meshComp.GetMesh().SetShader(shader);
						}
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::Separator();

				// Material
				auto material = meshComp.GetMesh().GetMaterial();
				std::string materialName = material ? material->GetName() : "None";
				ImGui::NextColumn();
				ImGui::Text("Material");
				ImGui::NextColumn();
				ImGui::Text("%s", materialName.c_str());

				// 显示材质属性
				{
					ImGui::Button("Texture", ImVec2{ 100.0f, 0.0f });

					// 拖拽添加纹理
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;

							// 检查扩展名
							if (texturePath.extension() == ".png" || texturePath.extension() == ".jpg" || texturePath.extension() == ".jpeg")
								material->SetAlbedoTexture(Texture2D::Create(texturePath.string()));
						}
						ImGui::EndDragDropTarget();
					}
				}

				ImGui::Columns(1);
		});
		ImGui::PopStyleColor();

		// Model Component
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.2f, 0.8f, 0.8f, 1.0f });
		DrawSingleComponent<ModelComponent>("Model Renderer", entity, [](auto& modelComp)
			{
				auto model = modelComp.m_Model;
				std::string modelPath = model ? model->GetPath() : "None";
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);
				ImGui::Text("Model");
				ImGui::NextColumn();
				ImGui::Button(modelPath.c_str(), ImVec2{ ImGui::GetContentRegionAvail().x, 0.0f });

				// 拖拽添加Model
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path modelPath = std::filesystem::path(g_AssetPath) / path;

						// 检查扩展名
						if (modelPath.extension() == ".obj" || modelPath.extension() == ".fbx")
						{
							auto model = Model::CreateFromFile(modelPath.string());
							modelComp.m_Model = model;
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::Columns(1);
			});
		ImGui::PopStyleColor();

		// Grid Component
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f });
		DrawSingleComponent<GridComponent>("Grid Renderer", entity, [](auto& gridComp)
			{
				auto primitiveType = gridComp.m_Grid->GetPrimitiveType();
				std::string primitiveTypeName;
				switch (primitiveType)
				{
				case VertexArray::PrimitiveType::Grid:
					primitiveTypeName = "Grid";
					break;
				default:
					primitiveTypeName = "None";
					break;
				};
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);
				ImGui::Text("Type");
				ImGui::NextColumn();
				ImGui::Text("%s", primitiveTypeName.c_str());
				if (primitiveType == VertexArray::PrimitiveType::Grid)
				{
					auto size = gridComp.m_Grid->GetGridSize();
					auto divisions = gridComp.m_Grid->GetGridDivisions();
					ImGui::NextColumn();
					ImGui::Text("Size");
					ImGui::NextColumn();
					ImGui::Text("%.2f", size);
					ImGui::NextColumn();
					ImGui::Text("Divisions");
					ImGui::NextColumn();
					ImGui::Text("%d", divisions);
				}
				ImGui::Columns(1);
			});
		ImGui::PopStyleColor();

		// Skybox Component
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f });
		DrawSingleComponent<SkyboxComponent>("Skybox", entity, [](auto& skyboxComp)
			{
				// 当前天空盒信息显示
				auto cubemap = skyboxComp.m_Cubemap;
				std::string cubemapPath = cubemap ? cubemap->GetPath() : "None";
				std::string shaderName = skyboxComp.m_Shader ? skyboxComp.m_Shader->GetName() : "None";

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);

				// Cubemap路径显示和选择
				ImGui::Text("Cubemap");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Text("%s", cubemapPath.c_str());

				// 添加选择按钮
				if (ImGui::Button("Select Cubemap", ImVec2(-1, 0)))
				{
					// 打开文件对话框选择天空盒目录
					std::string newPath = FileDialog::OpenFolder();
					if (!newPath.empty())
					{
						// 加载新的天空盒
						skyboxComp.m_Cubemap = CubemapTexture::Create(newPath);
					}
				}
				ImGui::PopItemWidth();

				// Shader信息
				ImGui::NextColumn();
				ImGui::Text("Shader");
				ImGui::NextColumn();
				ImGui::Text("%s", shaderName.c_str());

				// Shader选择按钮
				ImGui::PushItemWidth(-1);
				if (ImGui::Button("Select Shader", ImVec2(-1, 0)))
				{
					// 打开文件对话框选择Shader文件
					std::string shaderPath = FileDialog::OpenFile("GLSL Shader (*.glsl)\0*.glsl\0");
					if (!shaderPath.empty())
					{
						// 加载新的Shader
						skyboxComp.m_Shader = ShaderLibrary::Get().LoadSync(shaderPath);
					}
				}

				ImGui::Columns(1);
			});
		ImGui::PopStyleColor();

		// Script Component
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.5f, 0.2f, 0.7f, 1.0f });
		DrawSingleComponent<ScriptComponent>("Script", entity, [](auto& scriptComp)
			{
				bool isClassExists = ScriptEngine::EntityClassExists(scriptComp.Name);

				static char buffer[64];
				strcpy_s(buffer, scriptComp.Name.c_str());

				if(!isClassExists)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.92f, 0.3f, 0.2f, 1.0f });

				if (ImGui::InputText("Class Name", buffer, sizeof(buffer)))
					scriptComp.Name = std::string(buffer);

				if (!isClassExists)
					ImGui::PopStyleColor();
			});
		ImGui::PopStyleColor();
	}

	template<typename T>
	void SceneHierarchy::DisplayAddComponentEntry(const std::string& entryName) 
	{
		if (!m_SelectedEntity.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectedEntity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}