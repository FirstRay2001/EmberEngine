// ModelManager.cpp
// ģ�͹�����
// created by FirstRay2001, Oct/16/2025

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelManager.h"
#include "Source/Manager/TextureManager.h"
#include "Source/Manager/AnimationManager.h"
#include "Source/Scene/Component/Graphic/Model.h"
#include "Source/Scene/Component/Graphic/Mesh.h"
#include "Source/Scene/Component/Graphic/Material.h"
#include "Source/Scene/Component/Animator/Bone.h"
#include "Source/Scene/Component/Animator/Skeleton.h"
#include "Source/Scene/Component/Animator/Animation.h"

const std::string AssetDirectory = "D:/Dev/Project/EmberEngine/EmberEngine/Resources/Model/";

void MModelManager::Initialize()
{
}

int MModelManager::LoadModel(const std::string& ModelName, float ScaleFactor)
{
	std::string FormatStr = ModelName.substr(ModelName.find_last_of('.'));
	if (FormatStr == ".obj")
		return LoadModelOBJ(ModelName, ScaleFactor);
	if (FormatStr == ".fbx")
		return LoadModelFBX(ModelName, ScaleFactor);
	LOG_ERROR("Failed to load model: %s, unsupport format", ModelName.c_str());
	return 0;
}

int MModelManager::LoadModelOBJ(const std::string& ModelName, float ScaleFactor)
{
	// �Ƿ��Ѽ���
	int ModelIndex = FindModel(ModelName);
	if (ModelIndex != -1)
		return ModelIndex;

	// ����ģ��
	FModel* NewModel = LoadModelOBJInternal((AssetDirectory + ModelName).c_str(), ScaleFactor);

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

int MModelManager::LoadModelFBX(const std::string& ModelName, float ScaleFactor)
{
	// �Ƿ��Ѽ���
	int ModelIndex = FindModel(ModelName);
	if (ModelIndex != -1)
		return ModelIndex;

	// ����ģ��
	FModel* NewModel = LoadModelFBXInternal((AssetDirectory + ModelName).c_str(), ScaleFactor);

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

FModel* MModelManager::LoadModelOBJInternal(const char* ModelPath, float ScaleFactor)
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
	MySTL::TVector<MySTL::TSharedPtr<FMesh>> Meshs;
	MySTL::TVector<FMaterial> Materials;
	MySTL::TVector<unsigned int> MeshMaterialIndices;
	FProcessNodeInfo Info({ Meshs, Materials, MeshMaterialIndices, ModelDirectory, ScaleFactor });
	ProcessNodeOBJ(Scene->mRootNode, Scene, Info);

	// ����ģ��
	return new FModel(Info.Meshs, Info.Materials, Info.MeshMaterialIndices);
}

FModel* MModelManager::LoadModelFBXInternal(const char* ModelPath, float ScaleFactor)
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

	// ������Ƕ����
	ProcessEmbededTextures(Scene, ModelDirectory);

	// ����ڵ�
	MySTL::TVector<MySTL::TSharedPtr<FMesh>> Meshs;
	MySTL::TVector<FMaterial> Materials;
	MySTL::TVector<unsigned int> MeshMaterialIndices;
	FProcessNodeInfo Info({ Meshs, Materials, MeshMaterialIndices, ModelDirectory, ScaleFactor });
	ProcessNodeFBX(Scene->mRootNode, Scene, Info);

	// ���ɹ���
	FSkeleton* NewSkeleton = new FSkeleton();
	for (int i = 0; i < Info.Bones.Size(); i++)
	{
		FBone* Bone = Info.Bones[i];
		NewSkeleton->AddBone(MySTL::TSharedPtr<FBone>(Bone));
	}

	// ��ȡ�����㼶��ϵ
	SetBoneHierarchy(Scene->mRootNode, *NewSkeleton);

	// ע�����
	std::string ModelName = std::string(ModelPath).substr(std::string(ModelPath).find_last_of('/') + 1);
	MAnimationManager::GetInstance().AddSkeleton(ModelName, MySTL::TSharedPtr<FSkeleton>(NewSkeleton));

	// ������
	if (Scene->mNumAnimations > 0)
	{
		LOG_INFO("Processing animations for model: %s", ModelName.c_str());
		const aiAnimation* AAnimation = Scene->mAnimations[0];
		int ChannelSize = AAnimation->mNumChannels;
		float Duration = AAnimation->mDuration;

		// ��������
		FAnimation* NewAnimation = new FAnimation();
		NewAnimation->SetDuration(Duration);

		LOG_INFO("Animation has %d channels and duration %f", ChannelSize, Duration);

		// ÿ�������Ķ�������
		for (int i = 0; i < ChannelSize; i++)
		{
			auto Channel = AAnimation->mChannels[i];
			std::string BoneName = std::string(Channel->mNodeName.C_Str());

			// ������������
			if (Info.BoneMap.Find(BoneName) == nullptr)
			{
				LOG_WARN("Animation channel bone not found in skeleton: %s", BoneName.c_str());
				FBone* NewBone = new FBone();
				NewBone->SetBoneName(BoneName);
				Info.BoneMap[BoneName] = Info.Bones.Size();
				Info.Bones.push_back(NewBone);
				NewSkeleton->AddBone(MySTL::TSharedPtr<FBone>(NewBone));
			}

			// ��ȡ������������
			FAnimationClip AnimatonClip;
			AnimatonClip.SetTargetBoneName(BoneName);

			// λ�ùؼ�֡
			for (unsigned int j = 0; j < Channel->mNumPositionKeys; j++)
			{
				aiVector3D Position = Channel->mPositionKeys[j].mValue;
				float TimeStamp = static_cast<float>(Channel->mPositionKeys[j].mTime);
				AnimatonClip.AddPositionKeyFrame(MyMath::FVector3(Position.x, Position.y, Position.z), TimeStamp);
			}

			// ��ת�ؼ�֡
			for (unsigned int j = 0; j < Channel->mNumRotationKeys; j++)
			{
				aiQuaternion Rotation = Channel->mRotationKeys[j].mValue;
				float TimeStamp = static_cast<float>(Channel->mRotationKeys[j].mTime);
				AnimatonClip.AddRotationKeyFrame(MyMath::Quaternion(Rotation.w, Rotation.x, Rotation.y, Rotation.z), TimeStamp);
			}

			// ���Źؼ�֡
			for (unsigned int j = 0; j < Channel->mNumScalingKeys; j++)
			{
				aiVector3D Scale = Channel->mScalingKeys[j].mValue;
				float TimeStamp = static_cast<float>(Channel->mScalingKeys[j].mTime);
				AnimatonClip.AddScaleKeyFrame(MyMath::FVector3(Scale.x, Scale.y, Scale.z), TimeStamp);
			}

			// ��Ӷ���Ƭ�ε�����
			NewAnimation->AddAnimationClip(AnimatonClip);
		}

		// ע�ᶯ��
		MAnimationManager::GetInstance().AddAnimation(ModelName, MySTL::TSharedPtr<FAnimation>(NewAnimation));
	}

	// ����ģ��
	return new FModel(Info.Meshs, Info.Materials, Info.MeshMaterialIndices);
}

void MModelManager::ProcessNodeOBJ(aiNode* ANode, const aiScene* AScene, FProcessNodeInfo& Info)
{
	// ����ڵ���������
	for (unsigned int i = 0; i < ANode->mNumMeshes; i++)
	{
		aiMesh* AMesh = AScene->mMeshes[ANode->mMeshes[i]];
		ProcessMeshOBJ(AMesh, AScene, Info);
	}

	// �ݹ鴦���ӽڵ�
	for (unsigned int i = 0; i < ANode->mNumChildren; i++)
	{
		ProcessNodeOBJ(ANode->mChildren[i], AScene, Info);
	}
}

void MModelManager::ProcessNodeFBX(aiNode* ANode, const aiScene* AScene, FProcessNodeInfo& Info)
{
	// ����ڵ���������
	for (unsigned int i = 0; i < ANode->mNumMeshes; i++)
	{
		aiMesh* AMesh = AScene->mMeshes[ANode->mMeshes[i]];
		ProcessMeshFBX(AMesh, AScene, Info);
	}

	// �ݹ鴦���ӽڵ�
	for (unsigned int i = 0; i < ANode->mNumChildren; i++)
	{
		ProcessNodeFBX(ANode->mChildren[i], AScene, Info);
	}
}

void MModelManager::ProcessMeshOBJ(aiMesh* AMesh, const aiScene* AScene, FProcessNodeInfo& Info)
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

		FVertex Vertex = FMesh::CreateVertex(Position * Info.ScaleFactor, Normal, TexCoords, Tangent, BiTangent);
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
	FMesh* NewMesh = new FMesh(Vertices, Indices);
	Info.Meshs.emplace_back(NewMesh);

	// ��ȡ����
	aiMaterial* AMaterial = AScene->mMaterials[AMesh->mMaterialIndex];

	// ��ȡ����·��
	aiString DiffuseTexPath, SpecularTexPath, NormalPath;
	AMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &DiffuseTexPath);
	AMaterial->GetTexture(aiTextureType_SPECULAR, 0, &SpecularTexPath);
	AMaterial->GetTexture(aiTextureType_HEIGHT, 0, &NormalPath);

	std::string DiffusePathStr = Info.ModelDirectory + std::string(DiffuseTexPath.C_Str());
	std::string SpecularPathStr = Info.ModelDirectory + std::string(SpecularTexPath.C_Str());
	std::string NormalPathStr = Info.ModelDirectory + std::string(NormalPath.C_Str());

	// ����ȥ��
	for (unsigned int i = 0; i < Info.Materials.Size(); i++)
	{
		std::string ExistingDiffusePath = Info.Materials[i].GetDiffuseTexturePath();
		std::string ExistingSpecularPath = Info.Materials[i].GetSpecularTexturePath();
		std::string ExistingNormalPath = Info.Materials[i].GetNormalMapPath();
		if (ExistingDiffusePath == DiffusePathStr && ExistingSpecularPath == SpecularPathStr && ExistingNormalPath == NormalPathStr)
		{
			Info.MeshMaterialIndices.push_back(i);
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
	Info.Materials.push_back(NewMaterial);
	Info.MeshMaterialIndices.push_back(static_cast<unsigned int>(Info.Materials.Size() - 1));
}

void MModelManager::ProcessMeshFBX(aiMesh* AMesh, const aiScene* AScene, FProcessNodeInfo& Info)
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
		if (AMesh->mTangents)
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

		FVertex Vertex = FMesh::CreateVertex(Position * Info.ScaleFactor, Normal, TexCoords, Tangent, BiTangent);

		

		Vertices.push_back(Vertex);
	}

	// �������
	for(int BoneIndex = 0; BoneIndex < AMesh->mNumBones; BoneIndex++)
	{
		aiBone* ABone = AMesh->mBones[BoneIndex];
		std::string BoneName = ABone->mName.C_Str();

		// ��ȡƫ�ƾ���
		aiMatrix4x4 AOffsetMatrix = ABone->mOffsetMatrix;

		// ����¹���
		if (Info.BoneMap.Find(BoneName) == nullptr)
		{
			FBone* NewBone = new FBone();
			NewBone->SetBoneName(BoneName);
			MyMath::FMatrix OffsetMatrix(
				AOffsetMatrix.a1, AOffsetMatrix.a2, AOffsetMatrix.a3, AOffsetMatrix.a4,
				AOffsetMatrix.b1, AOffsetMatrix.b2, AOffsetMatrix.b3, AOffsetMatrix.b4,
				AOffsetMatrix.c1, AOffsetMatrix.c2, AOffsetMatrix.c3, AOffsetMatrix.c4,
				AOffsetMatrix.d1, AOffsetMatrix.d2, AOffsetMatrix.d3, AOffsetMatrix.d4
			);
			NewBone->SetOffsetMatrix(OffsetMatrix);
			Info.BoneMap[BoneName] = Info.Bones.Size();
			Info.Bones.push_back(NewBone);
		}

		// ������Ȩ��
		int BoneIndexGlobal = Info.BoneMap[BoneName];
		for (unsigned int WeightIndex = 0; WeightIndex < ABone->mNumWeights; WeightIndex++)
		{
			aiVertexWeight AWeight = ABone->mWeights[WeightIndex];
			size_t VertexId = AWeight.mVertexId;
			
			// �ҵ����в�λ
			for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
			{
				if (Vertices[VertexId].BoneWeight[j] == 0.0f)
				{
					// --Note: ��Index���ܻ��д�(��ȻĿǰû����)
					Vertices[VertexId].BoneID[j] = BoneIndexGlobal;
					Vertices[VertexId].BoneWeight[j] = AWeight.mWeight;
					break;
				}
			}
		}
	}

	// ��������
	for (unsigned int i = 0; i < AMesh->mNumFaces; i++)
	{
		aiFace Face = AMesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
			Indices.push_back(Face.mIndices[j]);
	}

	// ��������
	FMesh* NewMesh = new FMesh(Vertices, Indices);
	Info.Meshs.emplace_back(NewMesh);

	// ��ȡ����
	aiMaterial* AMaterial = AScene->mMaterials[AMesh->mMaterialIndex];

	// ��ȡ����·��
	aiString DiffuseTexPath, SpecularTexPath, NormalPath;

	AMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &DiffuseTexPath);
	AMaterial->GetTexture(aiTextureType_SPECULAR, 0, &SpecularTexPath);
	AMaterial->GetTexture(aiTextureType_NORMALS, 0, &NormalPath);
	std::string DiffusePathStr = Info.ModelDirectory + std::string(DiffuseTexPath.C_Str());
	std::string SpecularPathStr = Info.ModelDirectory + std::string(SpecularTexPath.C_Str());
	std::string NormalPathStr = Info.ModelDirectory + std::string(NormalPath.C_Str());

	// ����ȥ��
	for (unsigned int i = 0; i < Info.Materials.Size(); i++)
	{
		std::string ExistingDiffusePath = Info.Materials[i].GetDiffuseTexturePath();
		std::string ExistingSpecularPath = Info.Materials[i].GetSpecularTexturePath();
		std::string ExistingNormalPath = Info.Materials[i].GetNormalMapPath();
		if (ExistingDiffusePath == DiffusePathStr && ExistingSpecularPath == SpecularPathStr && ExistingNormalPath == NormalPathStr)
		{
			Info.MeshMaterialIndices.push_back(i);
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
	Info.Materials.push_back(NewMaterial);
	Info.MeshMaterialIndices.push_back(static_cast<unsigned int>(Info.Materials.Size() - 1));
}

void MModelManager::ProcessEmbededTextures(const aiScene* AScene, std::string ModelPath)
{
	if (AScene->mNumTextures == 0)
		return;

	for (unsigned int i = 0; i < AScene->mNumTextures; i++)
	{
		aiTexture* ATexture = AScene->mTextures[i];

		// PNG��JPG��ʽ
		if (ATexture->mHeight == 0)
		{
			std::string TextureName = ATexture->mFilename.length > 0 ? 
				(ModelPath + std::string(ATexture->mFilename.C_Str())) : (ModelPath + "_embeded_texture_" + std::to_string(i) + ".png");
			
			uint32_t TextureSize = ATexture->mWidth;

			if (TextureSize == 0)
			{
				LOG_WARN("Embeded texture has zero size in model: %s", ModelPath.c_str());
				continue;
			}

			// ������������
			unsigned char* TextureData = new unsigned char[TextureSize];
			memcpy(TextureData, ATexture->pcData, TextureSize);

			// �����������������
			MTextureManager::GetInstance().LoadTextureFromMemory(TextureName, TextureData, TextureSize);

			// �ͷ���ʱ����
			delete[] TextureData;

			LOG_INFO("Extract embeded texture: %s", TextureName.c_str());
		}
		// TODO:������ʽ
		else
		{
			LOG_WARN("Unsupported embeded texture format in model: %s", ModelPath.c_str());
		}
	}
}

void MModelManager::SetBoneHierarchy(aiNode* ANode, FSkeleton& Skeleton, int Depth) const
{
	// ��ȡ��ǰ����
	std::string CurrentNodeName = ANode->mName.C_Str();
	int CurrentBoneIndex = Skeleton.GetIndexOf(CurrentNodeName);
	MySTL::TSharedPtr<FBone> CurrentBonePtr;
	if(CurrentBoneIndex != -1)
		CurrentBonePtr = Skeleton.GetBone(CurrentBoneIndex);

	std::string Black(Depth, '|');
	// LOG_INFO("%s%s, Index: %d", Black.c_str(), CurrentNodeName.c_str(), CurrentBoneIndex);

	// ���ø�����
	if (Skeleton.GetRootIndex() < 0 && CurrentBoneIndex >= 0)
	{
		Skeleton.SetRootIndex(CurrentBoneIndex);
		// LOG_INFO("Set root bone: %s", CurrentBonePtr->GetBoneName().c_str());
	}

	// ���������ӽڵ�
	int ChildrenCnt = ANode->mNumChildren;
	for (int i = 0; i < ChildrenCnt; i++)
	{
		// ��ȡ�ӽڵ����
		std::string ChildrenName = ANode->mChildren[i]->mName.C_Str();
		int ChildrenIndex = Skeleton.GetIndexOf(ChildrenName);

		// ���ø��ӹ�ϵ
		if (CurrentBoneIndex != -1 && ChildrenIndex != -1)
		{
			auto ChildPtr = Skeleton.GetBone(ChildrenIndex);
			ChildPtr->SetParent(CurrentBonePtr);
			CurrentBonePtr->AddChild(ChildPtr);
		}

		// �㼶�ݹ�
		SetBoneHierarchy(ANode->mChildren[i], Skeleton, Depth + 1);
	}
}
		