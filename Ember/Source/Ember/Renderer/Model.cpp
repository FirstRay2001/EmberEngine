// Model.cpp
// 模型类
// created by FirstRay2001, Nov/25/2025

#include "emberpch.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Model.h"

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
		{
			model =  LoadOBJModel(filepath);
			model->m_Path = filepath.string();
		}
		// FBX格式
		else if (extension == ".fbx")
		{
			model = LoadFBXModel(filepath);
			model->m_Path = filepath.string();
		}
		else
		{
			EMBER_CORE_ERROR("Unsupported model format: {0}", extension);
		}

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

		ProcessNode(scene->mRootNode, scene, model, modelPath);

		return model;
	}

	Ref<Model> Model::LoadFBXModel(const std::filesystem::path& filepath)
	{
		EMBER_CORE_ERROR("FBX model loading not implemented yet.");
		return nullptr;
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, Ref<Model> model, const std::string& modelPath)
	{
		EMBER_CORE_TRACE("Processing node: {0}", node->mName.C_Str());
		// 处理节点中的所有网格
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Ref<Mesh> emberMesh = Mesh::CreateFromAssimpMesh(mesh, scene, modelPath);
			model->AddMesh(emberMesh);
		}

		// 递归处理子节点
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, model, modelPath);
		}
	}
}
