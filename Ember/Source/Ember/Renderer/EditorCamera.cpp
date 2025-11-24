// EditorCamera.cpp
// 编辑器相机
// created by FirstRay2001, Nov/23/2025


#include "emberpch.h"
#include "EditorCamera.h"
#include "Ember/Core/Input.h"
#include "Ember/Core/KeyCode.h"

namespace Ember
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) :
		m_Fov(fov),
		m_Aspect(aspectRatio),
		m_Near(nearClip),
		m_Far(farClip)
	{
		RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	}

	void EditorCamera::OnUpdate(const Timestep& timestep)
	{
		if (Input::IsKeyPressed(EMBER_KEY_LEFT_ALT))
		{
			if (!m_Initialized)
			{
				m_InitialMousePosition = { Input::GetMouseX(), Input::GetMouseY() };
				m_Initialized = true;
			}
			else
			{
				const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
				glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
				m_InitialMousePosition = mouse;

				if (Input::IsMouseButtonPressed(EMBER_MOUSE_BUTTON_MIDDLE))
					MousePan(delta);
				else if (Input::IsMouseButtonPressed(EMBER_MOUSE_BUTTON_LEFT))
					MouseRotate(delta);
				else if (Input::IsMouseButtonPressed(EMBER_MOUSE_BUTTON_RIGHT))
					MouseZoom(delta.y);

				RecalculateViewMatrix();
			}
		}
		else
		{
			m_Initialized = false;
		}
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		RecalculateViewMatrix();
		return false;
	}

	void EditorCamera::SetScreenSize(uint32_t width, uint32_t height)
	{
		if (!bInitialized)
		{
			bInitialized = true;
		}
		else
		{
			// 根据新的宽高比调整FOV
			m_Fov = glm::degrees(2.0f * atan(tan(glm::radians(m_Fov) / 2.0f) * ((float)height / (float)m_ScreenHeight)));
		}

		m_ScreenWidth = width;
		m_ScreenHeight = height;
		m_Aspect = (float)width / (float)height;
		RecalculateProjectionMatrix();
	}

	void EditorCamera::RecalculateViewMatrix()
	{
		// 重新计算位置
		RecalculatePosition();

		// 计算视图矩阵
		glm::quat rotation = GetRotation();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(rotation);

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void EditorCamera::RecalculateProjectionMatrix()
	{
		// 计算透视投影矩阵
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	void EditorCamera::RecalculatePosition()
	{
		m_Position = m_FocalPoint - m_Distance * GetForwardDirection();
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ScreenWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ScreenHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}
}