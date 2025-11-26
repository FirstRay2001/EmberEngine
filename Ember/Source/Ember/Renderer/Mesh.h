// Mesh.h
// 网格
// created by FirstRay2001, Nov/15/2025

#pragma once

#include "VertexArray.h"
#include "Material.h"
#include "Shader.h"
#include "Ember/Core/Core.h"

struct aiMesh;
struct aiScene;

namespace Ember
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const Ref<Shader>& shader)
			: m_VertexArray(vertexArray), m_Material(material), m_Shader(shader)
		{
		}
		virtual ~Mesh() = default;

		Ref <VertexArray> GetVertexArray() const { return m_VertexArray; }
		Ref<Material> GetMaterial() const { return m_Material; }
		Ref<Shader> GetShader() const { return m_Shader; }
		void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }

	public:
		static Ref<Mesh> CreateFromAssimpMesh(const aiMesh* mesh, const aiScene* scene, const std::string& modelPath);

	private:
		Ref<VertexArray>	m_VertexArray;
		Ref<Material>		m_Material;
		Ref<Shader>			m_Shader;
	};
}