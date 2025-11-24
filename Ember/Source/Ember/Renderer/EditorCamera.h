// EditorCamera.h
// 编辑器相机
// created by FirstRay2001, Nov/23/2025

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Ember/Core/Timestep.h"
#include "Ember/Events/MouseEvent.h"

namespace Ember
{
	// Note - 用于渲染编辑器场景的相机，只存在于编辑器模式，因此它不会作为一个组件存在，也不会依附于任何实体
	class EditorCamera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

	public:
		void OnUpdate(const Timestep& timestep);
		void OnEvent(Event& e);

	public:
		void SetScreenSize(uint32_t screenWidth, uint32_t screenHeight);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		float		GetAspectRatio() const { return m_Aspect; }
		void		SetAspectRatio(float aspect) { m_Aspect = aspect; RecalculateProjectionMatrix(); }
		float		GetFov() const { return m_Fov; }
		void		SetFov(float fov) { m_Fov = fov; RecalculateProjectionMatrix(); }
		float		GetNearClip() const { return m_Near; }
		void		SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjectionMatrix(); }
		float		GetFarClip() const { return m_Far; }
		void		SetFarClip(float farClip) { m_Far = farClip; RecalculateProjectionMatrix(); }
		const		glm::vec3& GetPosition() const { return m_Position; }
		void		SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		glm::quat	GetRotation() const { return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f)); }

		const glm::vec3 GetForwardDirection() const
		{
			return glm::normalize(GetRotation() * glm::vec3(0.0f, 0.0f, -1.0f));
		}

		const glm::vec3 GetRightDirection() const
		{
			return glm::normalize(GetRotation() * glm::vec3(1.0f, 0.0f, 0.0f));
		}

		const glm::vec3 GetUpDirection() const
		{
			return glm::normalize(GetRotation() * glm::vec3(0.0f, 1.0f, 0.0f));
		}

	private:
		// 重新计算视图矩阵  
		void RecalculateViewMatrix();

		// 重新计算投影矩阵  
		void RecalculateProjectionMatrix();

		// 重新计算相机位置
		void RecalculatePosition();

		std::pair<float, float> PanSpeed() const;

		float RotationSpeed() const;

		float ZoomSpeed() const;

		// 鼠标控制相机平移
		void MousePan(const glm::vec2& delta);

		// 鼠标控制相机旋转
		void MouseRotate(const glm::vec2& delta);

		// 鼠标控制相机距离
		void MouseZoom(float delta);

		bool OnMouseScroll(MouseScrolledEvent& e);


	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		float m_Aspect = 16.0f / 9.0f;
		float m_Fov = 45;
		float m_Near = 0.1f;
		float m_Far = 100.0f;

		bool bInitialized = false;
		uint32_t m_ScreenWidth;
		uint32_t m_ScreenHeight;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Yaw = 0.0f;
		float m_Pitch = glm::radians(30.0f);
		glm::vec2 m_InitialMousePosition = {0.0f, 0.0f};

		bool m_Initialized = false;
	};
}

