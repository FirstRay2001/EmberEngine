// Component.h
// ECS组件
// created by FirstRay2001, Nov/12/2025

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Ember/Renderer/Mesh.h>
#include <Ember/Renderer/Camera.h>

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
			glm::mat4 rotation =
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0, 0, 1));
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
		TagComponent(const std::string& tag)
			: Tag(tag)
		{
		}

		operator std::string& () { return Tag; }	// 是否合适?
	};

	// 网格组件
	struct MeshComponent
	{
		Mesh mesh;
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const Ref<Shader>& shader)
			: mesh(vertexArray, material, shader)
		{
		}

		const Mesh& GetMesh() const { return mesh; }
		Mesh& GetMesh() { return mesh; }
	};

	// 摄像机组件
	struct CameraComponent
	{
		Camera Camera;
		bool Primary = true; // 主摄像机标志
		bool FixedAspectRatio = false;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const Ember::Camera& camera)
			: Camera(camera)
		{
		}

		glm::mat4 GetViewMatrix() const
		{
			return Camera.GetViewMatrix();
		}

		glm::mat4 GetProjectionMatrix() const
		{
			return Camera.GetProjectionMatrix();
		}
	};
}