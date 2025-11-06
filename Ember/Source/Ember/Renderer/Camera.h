// Camera.h
// 相机类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include <glm/glm.hpp>

namespace Ember
{
	class Camera
	{
	public:
		Camera(float aspect_, float fov_ = 45.0f, float near_ = 0.1f, float far_ = 100.0f);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		float GetAspectRatio() const { return m_Aspect; }
		void SetAspectRatio(float aspect);
		float GetFov() const { return m_Fov; }
		void SetFov(float fov) { m_Fov = fov; RecalculateProjectionMatrix(); }

		void SetScreentSize(uint32_t width, uint32_t height);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		
	private:
		// 重新计算视图矩阵
		void RecalculateViewMatrix();

		// 重新计算投影矩阵
		void RecalculateProjectionMatrix();

	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		float m_Aspect;
		float m_Fov;
		float m_Near;
		float m_Far;
		bool bInitialized = false;
		uint32_t m_ScreenWidth;
		uint32_t m_ScreenHeight;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}

