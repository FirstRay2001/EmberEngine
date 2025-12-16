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
	class Skeleton;

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

		bool UseSkeleton() const { return m_Skeleton != nullptr; }
		Ref<Skeleton> GetSkeleton() const { return m_Skeleton; }
		Ref<Skeleton> GetSkeleton() { return m_Skeleton; }

	private:
		static Ref<Model> LoadOBJModel(const std::filesystem::path& filepath);
		static Ref<Model> LoadFBXModel(const std::filesystem::path& filepath);

		static void ProcessNode_OBJ(aiNode* node, const aiScene* scene, Ref<Model> model, const std::string& modelPath);
		static void ProcessNode_FBX(aiNode* node, const aiScene* scene, Ref<Model> model, const std::string& modelPath);

	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::string m_Path;
		Ref<Skeleton> m_Skeleton = nullptr;
	};
}
