// Model.h
// 模型类
// created by FirstRay2001, Nov/25/2025

#pragma once

#include <filesystem>
#include "Mesh.h"

struct aiNode;
struct aiScene;

namespace Ember
{
	class Model
	{
	public:
		Model() = default;
		Model(const std::vector<Ref<Mesh>>& meshes);
		virtual ~Model() = default;

		const std::string& GetPath() const { return m_Path; }
		const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
		void AddMesh(const Ref<Mesh>& mesh) { m_Meshes.push_back(mesh); }

		static Ref<Model> CreateFromFile(const std::filesystem::path& filepath);

	private:
		static Ref<Model> LoadOBJModel(const std::filesystem::path& filepath);
		static Ref<Model> LoadFBXModel(const std::filesystem::path& filepath);

		static void ProcessNode(aiNode* node, const aiScene* scene, Ref<Model> model, const std::string& modelPath);

	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::string m_Path;
	};
}
