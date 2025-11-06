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
		Camera(float aspect, float fov = 45.0f, float near = 0.1f, float far = 100.0f);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculativeViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculativeViewMatrix(); }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		
	private:
		// 重新计算视图矩阵
		void RecalculativeViewMatrix();

	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}

