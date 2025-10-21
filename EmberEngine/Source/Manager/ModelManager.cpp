// ModelManager.cpp
// 模型管理器
// created by FirstRay2001, Oct/16/2025

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelManager.h"
#include "Source/Scene/Component/Graphic/Model.h"
#include "Source/Scene/Component/Graphic/Mesh.h"
#include "Source/Scene/Component/Graphic/Material.h"

const std::string ModelDirectory = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Model/";

void MModelManager::Initialize()
{
}

int MModelManager::LoadModel(const std::string& ModelName)
{
	std::string FormatStr = ModelName.substr(ModelName.find_last_of('.'));
	if (FormatStr == ".obj")
		return LoadModelOBJ(ModelName);
	if (FormatStr == ".fbx")
		return LoadModelFBX(ModelName);
	LOG_ERROR("Failed to load model: %s, unsupport format", ModelName.c_str());
	return 0;
}

int MModelManager::LoadModelOBJ(const std::string& ModelName)
{
	// 是否已加载
	int ModelIndex = FindModel(ModelName);
	if (ModelIndex != -1)
		return ModelIndex;

	// 加载模型
	FModel* NewModel = LoadModelInternal((ModelDirectory + ModelName).c_str());

	// 添加到管理器
	if (NewModel == nullptr)
	{
		LOG_ERROR("Failed to load model: %s", ModelName.c_str());
		return -1;
	}

	int NewModelID = Models_.Size();
	Models_.emplace_back(NewModel);
	ModelMap_[ModelName] = NewModelID;
	return NewModelID;
}

int MModelManager::LoadModelFBX(const std::string& ModelName)
{
	LOG_WARN("not implemented: LoadModelFBX");
	return -1;
}

int MModelManager::FindModel(const std::string& ModelName) const
{
	if (ModelMap_.Find(ModelName) != nullptr)
		return ModelMap_[ModelName];
	else
		return -1;
}

MySTL::TSharedPtr<FModel> MModelManager::GetModel(int ModelID)
{
	return Models_[ModelID];
}

MySTL::TSharedPtr<FModel> MModelManager::GetModel(const std::string& ModelName)
{
	int ModelIndex = FindModel(ModelName);

	if (ModelIndex != -1)
		return Models_[ModelIndex];
	else
	{
		LOG_ERROR("Model not found: %s", ModelName.c_str());
		return MySTL::TSharedPtr<FModel>();
	}
}

FModel* MModelManager::LoadModelInternal(const char* ModelPath)
{
	// 读取场景
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(ModelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// 检测错误
	if (Scene == nullptr || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || Scene->mRootNode == nullptr)
	{
		LOG_ERROR("Assimp Error: %s", Importer.GetErrorString());
		return nullptr;
	}

	// 获取模型目录
	std::string ModelDirectory = std::string(ModelPath).substr(0, std::string(ModelPath).find_last_of('/') + 1);

	// 处理节点
	MySTL::TVector<FMesh> Meshs;
	MySTL::TVector<FMaterial> Materials;
	MySTL::TVector<unsigned int> MeshMaterialIndices;
	ProcessNode(Scene->mRootNode, Scene, Meshs, Materials, MeshMaterialIndices, ModelDirectory);

	// 生成模型
	return new FModel(Meshs, Materials, MeshMaterialIndices);
}

void MModelManager::ProcessNode(aiNode* ANode, const aiScene* AScene, MySTL::TVector<FMesh>& Meshs, MySTL::TVector<FMaterial>& Matrials, MySTL::TVector<unsigned int>& MeshMaterialIndices, std::string ModelDirectory)
{
	// 处理节点所有网格
	for (unsigned int i = 0; i < ANode->mNumMeshes; i++)
	{
		aiMesh* AMesh = AScene->mMeshes[ANode->mMeshes[i]];
		ProcessMesh(AMesh, AScene, Meshs, Matrials, MeshMaterialIndices, ModelDirectory);
	}

	// 递归处理子节点
	for (unsigned int i = 0; i < ANode->mNumChildren; i++)
	{
		ProcessNode(ANode->mChildren[i], AScene, Meshs, Matrials, MeshMaterialIndices, ModelDirectory);
	}
}

void MModelManager::ProcessMesh(aiMesh* AMesh, const aiScene* AScene, MySTL::TVector<FMesh>& Meshs, MySTL::TVector<FMaterial>& Materials, MySTL::TVector<unsigned int>& MeshMaterialIndices, std::string ModelDirectory)
{
	// 处理网格数据
	MySTL::TVector<FVertex> Vertices;
	MySTL::TVector<unsigned int> Indices;

	// 处理顶点
	for (unsigned int i = 0; i < AMesh->mNumVertices; i++)
	{
		// 位置
		MyMath::FVector3 Position(
			AMesh->mVertices[i].x,
			AMesh->mVertices[i].y,
			AMesh->mVertices[i].z);

		// 法线
		MyMath::FVector3 Normal(
			AMesh->mNormals[i].x,
			AMesh->mNormals[i].y,
			AMesh->mNormals[i].z);
		
		// 纹理坐标
		MyMath::FVector2 TexCoords(0.0f, 0.0f);
		if (AMesh->mTextureCoords[0])
		{
			TexCoords[0] = AMesh->mTextureCoords[0][i].x;
			TexCoords[1] = AMesh->mTextureCoords[0][i].y;
		}

		// 切线
		MyMath::FVector3 Tangent(1, 0, 0);
		if(AMesh->mTangents)
		{
			Tangent[0] = AMesh->mTangents[i].x;
			Tangent[1] = AMesh->mTangents[i].y;
			Tangent[2] = AMesh->mTangents[i].z;
		}

		// 副切线
		MyMath::FVector3 BiTangent(0, 1, 0);
		if (AMesh->mBitangents)
		{
			BiTangent[0] = AMesh->mBitangents[i].x;
			BiTangent[1] = AMesh->mBitangents[i].y;
			BiTangent[2] = AMesh->mBitangents[i].z;
		}

		FVertex Vertex = FMesh::CreateVertex(Position, Normal, TexCoords, Tangent, BiTangent);
		Vertices.push_back(Vertex);
	}

	// 处理索引
	for (unsigned int i = 0; i < AMesh->mNumFaces; i++)
	{
		aiFace Face = AMesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
			Indices.push_back(Face.mIndices[j]);
	}

	// 创建网格
	FMesh NewMesh(Vertices, Indices);
	Meshs.push_back(NewMesh);

	// 获取材质
	aiMaterial* AMaterial = AScene->mMaterials[AMesh->mMaterialIndex];

	// 获取纹理路径
	aiString DiffuseTexPath, SpecularTexPath, NormalPath;
	AMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &DiffuseTexPath);
	AMaterial->GetTexture(aiTextureType_SPECULAR, 0, &SpecularTexPath);
	AMaterial->GetTexture(aiTextureType_HEIGHT, 0, &NormalPath);

	std::string DiffusePathStr = ModelDirectory + std::string(DiffuseTexPath.C_Str());
	std::string SpecularPathStr = ModelDirectory + std::string(SpecularTexPath.C_Str());
	std::string NormalPathStr = ModelDirectory + std::string(NormalPath.C_Str());

	// 材质去重
	for (unsigned int i = 0; i < Materials.Size(); i++)
	{
		std::string ExistingDiffusePath = Materials[i].GetDiffuseTexturePath();
		std::string ExistingSpecularPath = Materials[i].GetSpecularTexturePath();
		std::string ExistingNormalPath = Materials[i].GetNormalMapPath();
		if (ExistingDiffusePath == DiffusePathStr && ExistingSpecularPath == SpecularPathStr && ExistingNormalPath == NormalPathStr)
		{
			MeshMaterialIndices.push_back(i);
			return;
		}
	}

	// 创建材质
	FMaterial NewMaterial;

	if (!std::string(DiffuseTexPath.C_Str()).empty())
		NewMaterial.SetDiffuse(DiffusePathStr);
	if (!std::string(SpecularTexPath.C_Str()).empty())
		NewMaterial.SetSpecular(SpecularPathStr);
	if (!std::string(NormalPath.C_Str()).empty())
		NewMaterial.SetNormal(NormalPathStr);
	Materials.push_back(NewMaterial);
	MeshMaterialIndices.push_back(static_cast<unsigned int>(Materials.Size() - 1));
}
		