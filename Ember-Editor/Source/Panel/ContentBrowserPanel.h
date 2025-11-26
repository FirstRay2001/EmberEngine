// ContentBrowserPanel.h
// 内容浏览器面板
// created by FirstRay2001, Nov/25/2025

#pragma once

#include <filesystem>

#include "Ember/Renderer/Texture.h"

namespace Ember
{
	class Scene;

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender();

	public:
		void SetContext(const Ref<Scene>& context) { m_Context = context; }

	private:
		// 根据文件类型获取图标
		Ref<Texture2D> GetIconForFile(const std::filesystem::path& path);

	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Scene> m_Context;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_ImageIcon;
		Ref<Texture2D> m_SceneIcon;
		Ref<Texture2D> m_PrefabIcon;
		Ref<Texture2D> m_ModelIcon;
	};
}
