// ToastPanel.h
// 消息提示浮窗
// created by FirstRay2001, Nov/27/2025

#include "emberpch.h"

#include <imgui/imgui.h>

#include "ToastPanel.h"

namespace Ember
{
	void ToastPanel::OnImGuiRender()
	{
#if 1
        if (m_Toasts.empty())
            return;

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoMove;

        // 设置位置
        ImVec2 toastWindowPos;
        if (m_ViewportMin.x != 0 && m_ViewportMin.y != 0)
        {
            // 使用Viewport坐标系
            toastWindowPos.x = m_ViewportMin.x + 10.0f;  // Viewport左上角 + 10像素偏移
            toastWindowPos.y = m_ViewportMin.y + 10.0f;
        }
        else if (m_ViewportPosition.x != 0 && m_ViewportPosition.y != 0)
        {
            // 使用Viewport位置和尺寸
            toastWindowPos.x = m_ViewportPosition.x + 10.0f;
            toastWindowPos.y = m_ViewportPosition.y + 10.0f;
        }
        else
        {
            // 回退到屏幕左上角
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            toastWindowPos.x = viewport->WorkOffsetMin.x + 10.0f;
            toastWindowPos.y = viewport->WorkOffsetMin.y + 10.0f;
        }

        ImGui::SetNextWindowPos(toastWindowPos, ImGuiCond_Always, ImVec2{0,0});

        // 开始渲染
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 8.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.8f));

        if (ImGui::Begin("Toasts", nullptr, flags))
        {
            RenderToastMessages();
        }

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
#endif
	}

	void ToastPanel::AddToast(const std::string& message, float duration, const ImVec4& color)
	{
		ToastInfo toast;
		toast.ID = m_NextToastID++;
		toast.Message = message;
		toast.Duration = duration;
		toast.Lifetime = duration;
		toast.Color = color;
		m_Toasts.push_back(toast);
	}

    void ToastPanel::AddToast_Info(const std::string& message)
    {
		EMBER_CORE_INFO("{}", message);
		AddToast(message, 3.0f, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
    }

    void ToastPanel::AddToast_Warn(const std::string& message)
    {
		EMBER_CORE_WARN("{}", message);
		AddToast(message, 5.0f, ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f });
    }

    void ToastPanel::AddToast_Error(const std::string& message)
    {
		EMBER_CORE_ERROR("{}", message);
		AddToast(message, 8.0f, ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    }

    void ToastPanel::RenderToastMessages()
    {
        float deltaTime = ImGui::GetIO().DeltaTime;
        auto it = m_Toasts.begin();

        while (it != m_Toasts.end())
        {
            ToastInfo& toast = *it;
            toast.Lifetime -= deltaTime;

            if (toast.Lifetime <= 0.0f)
            {
                // 消息过期，移除
                it = m_Toasts.erase(it);
                continue;
            }

            // 计算透明度
            float alpha = 1.0f;
            if (toast.Lifetime < 0.5f)
				alpha =  toast.Lifetime / 0.5f;  // 最后0.5秒淡出
            else if (toast.Duration - toast.Lifetime < 0.3f)
				alpha = (toast.Duration - toast.Lifetime) / 0.3f;   // 前0.3秒淡入

            // 设置颜色和透明度
            ImVec4 textColor = toast.Color;
            textColor.w = alpha;

            // 显示消息
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::TextUnformatted(toast.Message.c_str());
            ImGui::PopStyleColor();

            ++it;

            // 添加间距（除了最后一个）
            if (it != m_Toasts.end())
            {
                ImGui::Spacing();
            }
        }
    }

    void ToastPanel::SetViewportPosition(const ImVec2& viewportPos, const ImVec2& viewportSize)
    {
        m_ViewportPosition = viewportPos;
        m_ViewportSize = viewportSize;
    }

    void ToastPanel::SetViewportBounds(const ImVec2& viewportMin, const ImVec2& viewportMax)
    {
        m_ViewportMin = viewportMin;
        m_ViewportMax = viewportMax;
    }
}