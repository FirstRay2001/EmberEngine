// ContentBrowserPanel.h
// 内容浏览器面板
// created by FirstRay2001, Nov/25/2025

#pragma once

#include <filesystem>

#include "Ember/Renderer/Texture.h"

namespace Ember
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender();

	private:
		Ref<Texture2D> GetIconForFile(const std::filesystem::path& path);

	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_ImageIcon;
		Ref<Texture2D> m_PrefabIcon;
	};
}
