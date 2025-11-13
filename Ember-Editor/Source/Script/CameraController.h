// CameraController.h
// 相机控制脚本
// created by FirstRay2001, Nov/12/2025

#pragma once

#include "ScriptsCommon.h"

namespace Ember
{
	class CameraController : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override
		{
			auto& transform = GetComponent<TransformComponent>();
			transform.Position = { 0.0f, 0.0f, 5.0f };
			m_MousePosInitialized = false;
		}

		virtual void OnUpdate(Timestep timestep) override
		{
			if (!m_Enabled)
				return;

			// 获取前、右方向
			auto& transform = GetComponent<TransformComponent>();
			glm::vec3 forward = transform.GetTransform() * glm::vec4(0, 0, -1, 0);
			glm::vec3 right = transform.GetTransform() * glm::vec4(1, 0, 0, 0);

			// 控制移动
			if (Input::IsKeyPressed(EMBER_KEY_W))
				transform.Position += m_Speed * forward * timestep.GetSeconds();
			if (Input::IsKeyPressed(EMBER_KEY_S))
				transform.Position -= m_Speed * forward * timestep.GetSeconds();
			if (Input::IsKeyPressed(EMBER_KEY_A))
				transform.Position -= m_Speed * right * timestep.GetSeconds();
			if (Input::IsKeyPressed(EMBER_KEY_D))
				transform.Position += m_Speed * right * timestep.GetSeconds();

			// 控制旋转
			if (Input::IsMouseButtonPressed(EMBER_MOUSE_BUTTON_RIGHT))
			{
				// 避免第一次跳动
				if (!m_MousePosInitialized)
				{
					auto [mouseX, mouseY] = Input::GetMousePosition();
					m_PreviousMouseX = mouseX;
					m_PreviousMouseY = mouseY;
					m_MousePosInitialized = true;
				}
				else
				{
					auto [mouseX, mouseY] = Input::GetMousePosition();
					float deltaX = mouseX - m_PreviousMouseX;
					float deltaY = mouseY - m_PreviousMouseY;
					transform.Rotation.y -= deltaX * m_MouseSensitivity;
					transform.Rotation.x -= deltaY * m_MouseSensitivity;

					// 限制俯仰角度
					if (transform.Rotation.x > 89.0f)
						transform.Rotation.x = 89.0f;
					if (transform.Rotation.x < -89.0f)
						transform.Rotation.x = -89.0f;

					m_PreviousMouseX = mouseX;
					m_PreviousMouseY = mouseY;
				}	
			}
			else
			{
				m_MousePosInitialized = false;
			}
		}

		void SetEnable(bool value) { m_Enabled = value; }

	private:
		float m_Speed = 5.0f;
		bool m_Enabled = true;
		float m_MouseSensitivity = 0.1f;
		float m_PreviousMouseX = 0.0f;
		float m_PreviousMouseY = 0.0f;
		bool m_MousePosInitialized = false;
	};
}