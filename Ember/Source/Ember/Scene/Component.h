// Component.h
// ECS组件
// created by FirstRay2001, Nov/12/2025

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Ember/Renderer/Mesh.h"
#include "Ember/Renderer/Camera.h"
#include "Ember/Renderer/Light.h"
#include "ScriptableEntity.h"

namespace Ember
{
	// 变换组件
	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
		{
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
			glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);
			return translation * rotation * scale;
		}
	};

	// 标签组件
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) :
			Tag(tag)
		{
		}

		operator std::string& () { return Tag; }	// 是否合适?
	};

	// Mesh组件
	struct MeshComponent
	{
		Mesh Mesh;
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const Ref<Shader>& shader) :
			Mesh(vertexArray, material, shader)
		{
		}

		const Ember::Mesh& GetMesh() const { return Mesh; }
		Ember::Mesh& GetMesh() { return Mesh; }
	};

	// 网格组件
	struct GridComponent
	{
		Ref<VertexArray> m_Grid;
		Ref<Shader> m_Shader;
		GridComponent() = default;
		GridComponent(const GridComponent&) = default;
		GridComponent(const Ref<VertexArray>& grid, const Ref<Shader>& shader) :
			m_Grid(grid),
			m_Shader(shader)
		{
		}
	};

	// 摄像机组件
	struct CameraComponent
	{
		Camera m_Camera;
		bool m_IsPrimary = true; // 主摄像机标志
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const Ember::Camera& camera) :
			m_Camera(camera)
		{
		}

		glm::mat4 GetViewMatrix() const
		{
			return m_Camera.GetViewMatrix();
		}

		glm::mat4 GetProjectionMatrix() const
		{
			return m_Camera.GetProjectionMatrix();
		}
	};

	// 原生脚本组件
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	// 点光源组件
	struct PointLightComponent
	{
		PointLight m_PointLight;
		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent(const Ember::PointLight& pointLight) :
			m_PointLight(pointLight)
		{
		}
	};

	// 平行光组件
	struct DirectionalLightComponent
	{
		DirectionalLight m_DirectionalLight;
		DirectionalLightComponent() = default;
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
		DirectionalLightComponent(const Ember::DirectionalLight& directionalLight) :
			m_DirectionalLight(directionalLight)
		{
		}
	};
}