// Component.h
// ECS组件
// created by FirstRay2001, Nov/12/2025

#pragma once

#include "Ember/Core/UUID.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Ember/Renderer/Mesh.h"
#include "Ember/Renderer/Model.h"
#include "Ember/Renderer/Camera.h"
#include "Ember/Renderer/Light.h"
#include "Ember/Renderer/Texture.h"
#include "Ember/Renderer/Animation/Animation.h"

namespace Ember
{
	// ID组件
	struct IDComponent
	{
		UUID m_ID;
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID uuid) :
			m_ID(uuid)
		{
		}
	};

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

		glm::vec3 GetForward() const
		{
			return glm::normalize(glm::rotate(glm::quat(glm::radians(Rotation)), glm::vec3(0.0f, 0.0f, -1.0f)));
		}

		glm::vec3 GetRight() const
		{
			return glm::normalize(glm::rotate(glm::quat(glm::radians(Rotation)), glm::vec3(1.0f, 0.0f, 0.0f)));
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

	// Model组件
	struct ModelComponent
	{
		Ref<Model> m_Model = nullptr;
		ModelComponent() = default;
		ModelComponent(const ModelComponent&) = default;
		ModelComponent(const std::string& modelPath) :
			m_Model(Model::CreateFromFile(modelPath))
		{
		}
		ModelComponent(const Ref<Model>& model) :
			m_Model(model)
		{
		}
	};

	// Animator组件
	struct AnimatorComponent
	{
		Ref<Skeleton> m_BindingSkeleton = nullptr;
		std::vector<Ref<Animation>> m_Animations;
		int m_CurrentAnimationIndex = -1;

		AnimatorComponent() = default;
		AnimatorComponent(const AnimatorComponent&) = default;
		AnimatorComponent(const std::vector<Ref<Animation>>& animations) :
			m_Animations(animations)
		{
		}

		void AddAnimation(Ref<Animation>& animation)
		{
			animation->SetBindingSkeleton(m_BindingSkeleton);
			m_Animations.push_back(animation);

			// TEST 临时动画播放
			m_CurrentAnimationIndex = 0;
		}
	};

	// Grid组件
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

	// 天空盒组件
	struct SkyboxComponent
	{
		Ref<CubemapTexture> m_Cubemap = nullptr;
		Ref<Shader> m_Shader = nullptr;
		SkyboxComponent() = default;
		SkyboxComponent(const SkyboxComponent&) = default;
		SkyboxComponent(const Ref<CubemapTexture>& cubemap, const Ref<Shader>& shader) :
			m_Cubemap(cubemap),
			m_Shader(shader)
		{
		}
	};

	// 脚本组件
	struct ScriptComponent
	{
		std::string Name;
		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
		ScriptComponent(const std::string& name) :
			Name(name)
		{
		}
	};

	// 原生脚本组件
	class ScriptableEntity;
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

	//////// 物理组件 ////////

	
}