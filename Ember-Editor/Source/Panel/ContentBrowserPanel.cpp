// ContentBrowserPanel.h
// 内容浏览器面板
// created by FirstRay2001, Nov/25/2025

#include "emberpch.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "ContentBrowserPanel.h"
#include "Ember/Scene/SceneSerializer.h"

namespace Ember
{
	extern const std::filesystem::path g_AssetPath = "Asset";

	ContentBrowserPanel::ContentBrowserPanel() :
		m_CurrentDirectory(g_AssetPath)
	{
		m_FolderIcon = Texture2D::Create("Asset/Icon/icon_folder.png");
		m_FileIcon = Texture2D::Create("Asset/Icon/icon_file.png");
		m_ImageIcon = Texture2D::Create("Asset/Icon/icon_image.png");
		m_SceneIcon = Texture2D::Create("Asset/Icon/icon_scene.png");
		m_PrefabIcon = Texture2D::Create("Asset/Icon/icon_prefab.png");
		m_ModelIcon = Texture2D::Create("Asset/Icon/icon_model.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		// 返回上一级
		float aviWidth = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, aviWidth - 700.0f);
		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			ImGui::Columns(3);
			ImGui::SetColumnWidth(0, 50.0f);
			ImGui::SetColumnWidth(1, aviWidth - 750.0f);
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
			ImGui::NextColumn();
		}

		// 当前目录
		ImGui::Text("Current Directory: %s", m_CurrentDirectory.string().c_str());

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		// 调节缩略图大小和间距
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.0f, 512.0f);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::SliderFloat("Padding", &padding, 0, 32);
		ImGui::PopItemWidth();
		ImGui::Columns(1);

		ImGui::Separator();

		// 获取文件列表的位置
		ImVec2 fileListStartPos = ImGui::GetCursorScreenPos();

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = entry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filename = relativePath.filename().string();

			Ref<Texture2D> icon = GetIconForFile(path);
			ImGui::PushID(filename.c_str());

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((void*)(uintptr_t)icon->GetRendererID(), ImVec2(thumbnailSize, thumbnailSize), ImVec2(0, 1), ImVec2(1, 0), 0);
			ImGui::PopStyleColor();

			// 拖拽
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::Text("%s", filename.c_str());
				ImGui::EndDragDropSource();
			}

			// 双击打开
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (std::filesystem::is_directory(path))
				{
					m_CurrentDirectory /= path.filename();
				}
				else
				{
					EMBER_CORE_INFO("Open file: {0}", path.string());
				}
			}
			ImGui::TextWrapped("%s", filename.c_str());
			ImGui::PopID();
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		// 处理空白区域的拖拽
		ImGui::SetCursorScreenPos(fileListStartPos);
		ImGui::InvisibleButton("##EmptySpaceTarget", ImVec2(-1, -1));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
			{
				const uint32_t entityId = *(const uint32_t*)payload->Data;
				if (m_Context)
				{
					SceneSerializer serializer(m_Context);
					std::filesystem::path prefabPath = m_CurrentDirectory / ("Entity_" + std::to_string(entityId) + ".prefab");
					serializer.SerializePrefab(entityId, prefabPath.string());
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::End();
	}

	Ref<Texture2D> ContentBrowserPanel::GetIconForFile(const std::filesystem::path& path)
	{
		if (std::filesystem::is_directory(path))
			return m_FolderIcon;
		else if (path.extension() == ".png" || path.extension() == ".jpg" || path.extension() == ".jpeg")
			return m_ImageIcon;
		else if (path.extension() == ".ember")
			return m_SceneIcon;
		else if (path.extension() == ".prefab")
			return m_PrefabIcon;
		else if (path.extension() == ".obj" || path.extension() == ".fbx")
			return m_ModelIcon;
		else
			return m_FileIcon;
	}
}