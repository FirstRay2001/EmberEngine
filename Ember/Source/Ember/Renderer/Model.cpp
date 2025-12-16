// Model.cpp
// 模型类
// created by FirstRay2001, Nov/25/2025

#include "emberpch.h"
#include "Model.h"
#include "Animation/Skeleton.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Ember
{
	extern const std::filesystem::path g_AssetPath;

	Model::Model(const std::vector<Ref<Mesh>>& meshes)
		: m_Meshes(meshes)
	{
	}

	Ref<Model> Model::CreateFromFile(const std::filesystem::path& filepath)
	{
		// 获取文件扩展名
		auto extension = filepath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
		Ref<Model> model = nullptr;

		// OBJ格式
		if (extension == ".obj")
			model =  LoadOBJModel(filepath);
		// FBX格式
		else if (extension == ".fbx")
			model = LoadFBXModel(filepath);
		else
			EMBER_CORE_ERROR("Unsupported model format: {0}", extension);

		if (model)
			model->m_Path = filepath.string();

		return model;
	}

	Ref<Model> Model::LoadOBJModel(const std::filesystem::path& filepath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filepath.string(),
			aiProcess_Triangulate |
			//aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			EMBER_CORE_ERROR("Assimp Error: {0}", importer.GetErrorString());
			return nullptr;
		}

		Ref<Model> model = CreateRef<Model>();

		// 处理根节点
		auto modelPath = filepath.parent_path().string();

		ProcessNode_OBJ(scene->mRootNode, scene, model, modelPath);

		return model;
	}	

	void Model::ProcessNode_OBJ(aiNode* node, const aiScene* scene, Ref<Model> model, const std::string& modelPath)
	{
		// 处理节点中的所有网格
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Ref<Mesh> emberMesh = Mesh::CreateFromAssimpMesh_OBJ(mesh, scene, modelPath);
			model->AddMesh(emberMesh);
		}

		// 递归处理子节点
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode_OBJ(node->mChildren[i], scene, model, modelPath);
		}
	}

	Ref<Model> Model::LoadFBXModel(const std::filesystem::path& filepath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filepath.string(),
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			EMBER_CORE_ERROR("Assimp Error: {0}", importer.GetErrorString());
			return nullptr;
		}

		Ref<Model> model = CreateRef<Model>();

		// 处理骨骼
		model->m_Skeleton = Skeleton::CreateFromAssimpScene_FBX(scene);

		// 处理根节点
		auto modelPath = filepath.parent_path().string();
		ProcessNode_FBX(scene->mRootNode, scene, model, modelPath);

		return model;
	}

	void Model::ProcessNode_FBX(aiNode* node, const aiScene* scene, Ref<Model> model, const std::string& modelPath)
	{
		// 处理节点中的所有网格
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Ref<Mesh> emberMesh = Mesh::CreateFromAssimpMesh_FBX(mesh, scene, modelPath, model);
			model->AddMesh(emberMesh);
		}

		// 递归处理子节点
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode_FBX(node->mChildren[i], scene, model, modelPath);
		}
	}
}
