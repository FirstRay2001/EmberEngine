// Light.h
// 光照
// created by FirstRay2001, Nov/6/2025

#pragma once

#include <glm/glm.hpp>

namespace Ember
{
	// 平行光数据
	struct DirectionalLight
	{
		glm::vec3 Direction		= { 0.0f, -1.0f, 0.0f };
		glm::vec3 Ambient		= { 0.0f, 0.0f, 0.0f };
		glm::vec3 Diffuse		= { 0.0f, 0.0f, 0.0f };
		glm::vec3 Specular		= { 0.0f, 0.0f, 0.0f };
	};

	// 点光源数据
	struct PointLight
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Ambient = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Diffuse = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Specular = { 0.0f, 0.0f, 0.0f };

		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;
	};

	// 聚光灯数据
	struct SpotLight
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
		glm::vec3 Ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 Diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 Specular = { 1.0f, 1.0f, 1.0f };

		float CutOff = glm::cos(glm::radians(12.5f));
		float OuterCutOff = glm::cos(glm::radians(15.0f));

		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;
	};
}