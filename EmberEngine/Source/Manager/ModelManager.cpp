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
	// ��ȡ����
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(ModelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

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
		// λ��
		MyMath::FVector3 Position(
			AMesh->mVertices[i].x,
			AMesh->mVertices[i].y,
			AMesh->mVertices[i].z);

		// ����
		MyMath::FVector3 Normal(
			AMesh->mNormals[i].x,
			AMesh->mNormals[i].y,
			AMesh->mNormals[i].z);
		
		// ��������
		MyMath::FVector2 TexCoords(0.0f, 0.0f);
		if (AMesh->mTextureCoords[0])
		{
			TexCoords[0] = AMesh->mTextureCoords[0][i].x;
			TexCoords[1] = AMesh->mTextureCoords[0][i].y;
		}

		// ����
		MyMath::FVector3 Tangent(1, 0, 0);
		if(AMesh->mTangents)
		{
			Tangent[0] = AMesh->mTangents[i].x;
			Tangent[1] = AMesh->mTangents[i].y;
			Tangent[2] = AMesh->mTangents[i].z;
		}

		// ������
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
	aiString DiffuseTexPath, SpecularTexPath, NormalPath;
	AMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &DiffuseTexPath);
	AMaterial->GetTexture(aiTextureType_SPECULAR, 0, &SpecularTexPath);
	AMaterial->GetTexture(aiTextureType_HEIGHT, 0, &NormalPath);

	std::string DiffusePathStr = ModelDirectory + std::string(DiffuseTexPath.C_Str());
	std::string SpecularPathStr = ModelDirectory + std::string(SpecularTexPath.C_Str());
	std::string NormalPathStr = ModelDirectory + std::string(NormalPath.C_Str());

	// ����ȥ��
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

	// ��������
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
		