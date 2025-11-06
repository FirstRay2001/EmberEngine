// Camera.cpp
// 相机类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Ember
{
	Camera::Camera(float aspect_, float fov_, float near_, float far_) :
		m_Aspect(aspect_),
		m_Fov(fov_),
		m_Near(near_),
		m_Far(far_)
	{
		RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	}

	void Camera::SetScreentSize(uint32_t width, uint32_t height)
	{
		if (!bInitialized)
		{
			bInitialized = true;
		}
		else
		{
			// 是否根据新的宽高比调整FOV
			// m_Fov = glm::degrees(2.0f * atan(tan(glm::radians(m_Fov) / 2.0f) * ((float)height / (float)m_ScreenHeight)));
		}

		m_ScreenWidth = width;
		m_ScreenHeight = height;
		m_Aspect = (float)width / (float)height;
		RecalculateProjectionMatrix();
	}

	void Camera::SetAspectRatio(float aspect)
	{
		m_Aspect = aspect;
		RecalculateProjectionMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		// 计算视图矩阵
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateProjectionMatrix()
	{
		// 计算透视投影矩阵
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}