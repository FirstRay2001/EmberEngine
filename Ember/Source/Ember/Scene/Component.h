// Component.h
// ECS组件
// created by FirstRay2001, Nov/12/2025

#pragma once

#include <glm/glm.hpp>
#include <Ember/Renderer/Mesh.h>

namespace Ember
{
	// 变换组件
	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) 
		{
		}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
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
}