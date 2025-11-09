// Light.h
// 光照
// created by FirstRay2001, Nov/6/2025

#pragma once

#include <glm/glm.hpp>

namespace Ember
{
	// 平行光数据
	struct DiretionalLight
	{
		glm::vec3 m_Direction		= { -0.2f, -1.0f, -0.3f };
		glm::vec3 m_Ambient		= { 0.2f, 0.2f, 0.2f };
		glm::vec3 m_Diffuse		= { 0.5f, 0.5f, 0.5f };
		glm::vec3 m_Specular		= { 1.0f, 1.0f, 1.0f };
	};

	// 点光源数据
	struct PointLight
	{
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 m_Diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 m_Specular = { 1.0f, 1.0f, 1.0f };

		float m_Constant = 1.0f;
		float m_Linear = 0.09f;
		float m_Quadratic = 0.032f;
	};

	// 聚光灯数据
	struct SpotLight
	{
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Direction = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 m_Diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 m_Specular = { 1.0f, 1.0f, 1.0f };

		float m_CutOff = glm::cos(glm::radians(12.5f));
		float m_OuterCutOff = glm::cos(glm::radians(15.0f));

		float m_Constant = 1.0f;
		float m_Linear = 0.09f;
		float m_Quadratic = 0.032f;
	};
}