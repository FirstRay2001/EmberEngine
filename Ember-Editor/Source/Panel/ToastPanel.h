// ToastPanel.h
// 消息提示浮窗
// created by FirstRay2001, Nov/27/2025

#pragma once

namespace Ember
{
	struct ToastInfo
	{
		uint32_t ID;
		std::string Message;
		float Duration;
		ImVec4 Color;
		float Lifetime = 0.0f;
	};

	class ToastPanel
	{
	public:
		ToastPanel() = default;
		void OnImGuiRender();

	public:
		// 添加一条提示消息
		void AddToast(const std::string& message, float duration = 3.0f, const ImVec4& color = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

		// 快捷封装，会调用LogInfo
		void AddToast_Info(const std::string& message);

		// 快捷封装，会调用LogWarn
		void AddToast_Warn(const std::string& message);

		// 快捷封装，会调用LogError
		void AddToast_Error(const std::string& message);

		void SetViewportPosition(const ImVec2& viewportPos, const ImVec2& viewportSize);
		void SetViewportBounds(const ImVec2& viewportMin, const ImVec2& viewportMax);

	private:
		void RenderToastMessages();

	private:
		std::deque<ToastInfo> m_Toasts;
		size_t m_MaxToasts = 5;			// 最大同时显示的提示数量
		uint32_t m_NextToastID = 1;

		ImVec2 m_ViewportPosition;  // Viewport在屏幕中的位置
		ImVec2 m_ViewportSize;      // Viewport尺寸
		ImVec2 m_ViewportMin;
		ImVec2 m_ViewportMax;
	};
}
