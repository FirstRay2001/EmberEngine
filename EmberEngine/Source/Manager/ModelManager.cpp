// ModelManager.cpp
// ģ�͹�����
// created by FirstRay2001, Oct/16/2025

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelManager.h"
#include "Source/Scene/Component/Graphic/Model.h"
#include "Source/Scene/Component/Graphic/Mesh.h"
#include "Source/Scene/Component/Graphic/Material.h"

const std::string ModelDirectory = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Model/";

int MModelManager::LoadModel(const std::string& ModelName)
{
	// �Ƿ��Ѽ���
	int ModelIndex = FindModel(ModelName);
	if (ModelIndex != -1)
		return ModelIndex;

	// ����ģ��
	FModel* NewModel = LoadModelInternal((ModelDirectory + ModelName).c_str());

	// ��ӵ�������
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
	// ��ȡ����
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(ModelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	// ������
	if (Scene == nullptr || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || Scene->mRootNode == nullptr)
	{
		LOG_ERROR("Assimp Error: %s", Importer.GetErrorString());
		return nullptr;
	}

	// ��ȡģ��Ŀ¼
	std::string ModelDirectory = std::string(ModelPath).substr(0, std::string(ModelPath).find_last_of('/') + 1);

	// ����ڵ�
	MySTL::TVector<FMesh> Meshs;
	MySTL::TVector<FMaterial> Materials;
	MySTL::TVector<unsigned int> MeshMaterialIndices;
	ProcessNode(Scene->mRootNode, Scene, Meshs, Materials, MeshMaterialIndices, ModelDirectory);

	// ����ģ��
	return new FModel(Meshs, Materials, MeshMaterialIndices);
}

void MModelManager::ProcessNode(aiNode* ANode, const aiScene* AScene, MySTL::TVector<FMesh>& Meshs, MySTL::TVector<FMaterial>& Matrials, MySTL::TVector<unsigned int>& MeshMaterialIndices, std::string ModelDirectory)
{
	// ����ڵ���������
	for (unsigned int i = 0; i < ANode->mNumMeshes; i++)
	{
		aiMesh* AMesh = AScene->mMeshes[ANode->mMeshes[i]];
		ProcessMesh(AMesh, AScene, Meshs, Matrials, MeshMaterialIndices, ModelDirectory);
	}

	// �ݹ鴦���ӽڵ�
	for (unsigned int i = 0; i < ANode->mNumChildren; i++)
	{
		ProcessNode(ANode->mChildren[i], AScene, Meshs, Matrials, MeshMaterialIndices, ModelDirectory);
	}
}

void MModelManager::ProcessMesh(aiMesh* AMesh, const aiScene* AScene, MySTL::TVector<FMesh>& Meshs, MySTL::TVector<FMaterial>& Materials, MySTL::TVector<unsigned int>& MeshMaterialIndices, std::string ModelDirectory)
{
	// ������������
	MySTL::TVector<FVertex> Vertices;
	MySTL::TVector<unsigned int> Indices;

	// ������
	for (unsigned int i = 0; i < AMesh->mNumVertices; i++)
	{
		MyMath::FVector3 Position(
			AMesh->mVertices[i].x,
			AMesh->mVertices[i].y,
			AMesh->mVertices[i].z);

		MyMath::FVector3 Normal(
			AMesh->mNormals[i].x,
			AMesh->mNormals[i].y,
			AMesh->mNormals[i].z);

		MyMath::FVector2 TexCoords(0.0f, 0.0f);
		if (AMesh->mTextureCoords[0]) // ����Ƿ�����������
		{
			TexCoords[0] = AMesh->mTextureCoords[0][i].x;
			TexCoords[1] = AMesh->mTextureCoords[0][i].y;
		}

		FVertex Vertex = FMesh::CreateVertex(Position, Normal, TexCoords);
		Vertices.push_back(Vertex);
	}

	// ��������
	for (unsigned int i = 0; i < AMesh->mNumFaces; i++)
	{
		aiFace Face = AMesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
			Indices.push_back(Face.mIndices[j]);
	}

	// ��������
	FMesh NewMesh(Vertices, Indices);
	Meshs.push_back(NewMesh);

	// ��ȡ����
	aiMaterial* AMaterial = AScene->mMaterials[AMesh->mMaterialIndex];

	// ��ȡ����·��
	aiString DiffuseTexPath, SpecularTexPath;
	AMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &DiffuseTexPath);
	AMaterial->GetTexture(aiTextureType_SPECULAR, 0, &SpecularTexPath);

	std::string DiffusePathStr = ModelDirectory + std::string(DiffuseTexPath.C_Str());
	std::string SpecularPathStr = ModelDirectory + std::string(SpecularTexPath.C_Str());

	// ����ȥ��
	for (unsigned int i = 0; i < Materials.Size(); i++)
	{
		std::string ExistingDiffusePath = Materials[i].GetDiffuseTexturePath();
		std::string ExistingSpecularPath = Materials[i].GetSpecularTexturePath();
		if (ExistingDiffusePath == DiffusePathStr && ExistingSpecularPath == SpecularPathStr)
		{
			MeshMaterialIndices.push_back(i);
			return;
		}
	}

	// ��������
	FMaterial NewMaterial(DiffusePathStr, SpecularPathStr, 32.0f);
	Materials.push_back(NewMaterial);
	MeshMaterialIndices.push_back(static_cast<unsigned int>(Materials.Size() - 1));
}