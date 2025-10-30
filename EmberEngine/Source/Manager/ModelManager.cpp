// ModelManager.cpp
// 模型管理器
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
	// 是否已加载
	int ModelIndex = FindModel(ModelName);
	if (ModelIndex != -1)
		return ModelIndex;

	// 加载模型
	FModel* NewModel = LoadModelOBJInternal((AssetDirectory + ModelName).c_str(), ScaleFactor);

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

int MModelManager::LoadModelFBX(const std::string& ModelName, float ScaleFactor)
{
	// 是否已加载
	int ModelIndex = FindModel(ModelName);
	if (ModelIndex != -1)
		return ModelIndex;

	// 加载模型
	FModel* NewModel = LoadModelFBXInternal((AssetDirectory + ModelName).c_str(), ScaleFactor);

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
	MySTL::TVector<MySTL::TSharedPtr<FMesh>> Meshs;
	MySTL::TVector<FMaterial> Materials;
	MySTL::TVector<unsigned int> MeshMaterialIndices;
	FProcessNodeInfo Info({ Meshs, Materials, MeshMaterialIndices, ModelDirectory, ScaleFactor });
	ProcessNodeOBJ(Scene->mRootNode, Scene, Info);

	// 生成模型
	return new FModel(Info.Meshs, Info.Materials, Info.MeshMaterialIndices);
}

FModel* MModelManager::LoadModelFBXInternal(const char* ModelPath, float ScaleFactor)
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

	// 加载内嵌纹理
	ProcessEmbededTextures(Scene, ModelDirectory);

	// 处理节点
	MySTL::TVector<MySTL::TSharedPtr<FMesh>> Meshs;
	MySTL::TVector<FMaterial> Materials;
	MySTL::TVector<unsigned int> MeshMaterialIndices;
	FProcessNodeInfo Info({ Meshs, Materials, MeshMaterialIndices, ModelDirectory, ScaleFactor });
	ProcessNodeFBX(Scene->mRootNode, Scene, Info);

	// 生成骨骼
	FSkeleton* NewSkeleton = new FSkeleton();
	for (int i = 0; i < Info.Bones.Size(); i++)
	{
		FBone* Bone = Info.Bones[i];
		NewSkeleton->AddBone(MySTL::TSharedPtr<FBone>(Bone));
	}

	// 获取骨骼层级关系
	SetBoneHierarchy(Scene->mRootNode, *NewSkeleton);

	// 注册骨骼
	std::string ModelName = std::string(ModelPath).substr(std::string(ModelPath).find_last_of('/') + 1);
	MAnimationManager::GetInstance().AddSkeleton(ModelName, MySTL::TSharedPtr<FSkeleton>(NewSkeleton));

	// 处理动画
	if (Scene->mNumAnimations > 0)
	{
		LOG_INFO("Processing animations for model: %s", ModelName.c_str());
		const aiAnimation* AAnimation = Scene->mAnimations[0];
		int ChannelSize = AAnimation->mNumChannels;
		float Duration = AAnimation->mDuration;

		// 创建动画
		FAnimation* NewAnimation = new FAnimation();
		NewAnimation->SetDuration(Duration);

		LOG_INFO("Animation has %d channels and duration %f", ChannelSize, Duration);

		// 每根骨骼的动画数据
		for (int i = 0; i < ChannelSize; i++)
		{
			auto Channel = AAnimation->mChannels[i];
			std::string BoneName = std::string(Channel->mNodeName.C_Str());

			// 检测骨骼完整性
			if (Info.BoneMap.Find(BoneName) == nullptr)
			{
				LOG_WARN("Animation channel bone not found in skeleton: %s", BoneName.c_str());
				FBone* NewBone = new FBone();
				NewBone->SetBoneName(BoneName);
				Info.BoneMap[BoneName] = Info.Bones.Size();
				Info.Bones.push_back(NewBone);
				NewSkeleton->AddBone(MySTL::TSharedPtr<FBone>(NewBone));
			}

			// 读取骨骼动画数据
			FAnimationClip AnimatonClip;
			AnimatonClip.SetTargetBoneName(BoneName);

			// 位置关键帧
			for (unsigned int j = 0; j < Channel->mNumPositionKeys; j++)
			{
				aiVector3D Position = Channel->mPositionKeys[j].mValue;
				float TimeStamp = static_cast<float>(Channel->mPositionKeys[j].mTime);
				AnimatonClip.AddPositionKeyFrame(MyMath::FVector3(Position.x, Position.y, Position.z), TimeStamp);
			}

			// 旋转关键帧
			for (unsigned int j = 0; j < Channel->mNumRotationKeys; j++)
			{
				aiQuaternion Rotation = Channel->mRotationKeys[j].mValue;
				float TimeStamp = static_cast<float>(Channel->mRotationKeys[j].mTime);
				AnimatonClip.AddRotationKeyFrame(MyMath::Quaternion(Rotation.w, Rotation.x, Rotation.y, Rotation.z), TimeStamp);
			}

			// 缩放关键帧
			for (unsigned int j = 0; j < Channel->mNumScalingKeys; j++)
			{
				aiVector3D Scale = Channel->mScalingKeys[j].mValue;
				float TimeStamp = static_cast<float>(Channel->mScalingKeys[j].mTime);
				AnimatonClip.AddScaleKeyFrame(MyMath::FVector3(Scale.x, Scale.y, Scale.z), TimeStamp);
			}

			// 添加动画片段到动画
			NewAnimation->AddAnimationClip(AnimatonClip);
		}

		// 注册动画
		MAnimationManager::GetInstance().AddAnimation(ModelName, MySTL::TSharedPtr<FAnimation>(NewAnimation));
	}

	// 生成模型
	return new FModel(Info.Meshs, Info.Materials, Info.MeshMaterialIndices);
}

void MModelManager::ProcessNodeOBJ(aiNode* ANode, const aiScene* AScene, FProcessNodeInfo& Info)
{
	// 处理节点所有网格
	for (unsigned int i = 0; i < ANode->mNumMeshes; i++)
	{
		aiMesh* AMesh = AScene->mMeshes[ANode->mMeshes[i]];
		ProcessMeshOBJ(AMesh, AScene, Info);
	}

	// 递归处理子节点
	for (unsigned int i = 0; i < ANode->mNumChildren; i++)
	{
		ProcessNodeOBJ(ANode->mChildren[i], AScene, Info);
	}
}

void MModelManager::ProcessNodeFBX(aiNode* ANode, const aiScene* AScene, FProcessNodeInfo& Info)
{
	// 处理节点所有网格
	for (unsigned int i = 0; i < ANode->mNumMeshes; i++)
	{
		aiMesh* AMesh = AScene->mMeshes[ANode->mMeshes[i]];
		ProcessMeshFBX(AMesh, AScene, Info);
	}

	// 递归处理子节点
	for (unsigned int i = 0; i < ANode->mNumChildren; i++)
	{
		ProcessNodeFBX(ANode->mChildren[i], AScene, Info);
	}
}

void MModelManager::ProcessMeshOBJ(aiMesh* AMesh, const aiScene* AScene, FProcessNodeInfo& Info)
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

		FVertex Vertex = FMesh::CreateVertex(Position * Info.ScaleFactor, Normal, TexCoords, Tangent, BiTangent);
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
	FMesh* NewMesh = new FMesh(Vertices, Indices);
	Info.Meshs.emplace_back(NewMesh);

	// 获取材质
	aiMaterial* AMaterial = AScene->mMaterials[AMesh->mMaterialIndex];

	// 获取纹理路径
	aiString DiffuseTexPath, SpecularTexPath, NormalPath;
	AMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &DiffuseTexPath);
	AMaterial->GetTexture(aiTextureType_SPECULAR, 0, &SpecularTexPath);
	AMaterial->GetTexture(aiTextureType_HEIGHT, 0, &NormalPath);

	std::string DiffusePathStr = Info.ModelDirectory + std::string(DiffuseTexPath.C_Str());
	std::string SpecularPathStr = Info.ModelDirectory + std::string(SpecularTexPath.C_Str());
	std::string NormalPathStr = Info.ModelDirectory + std::string(NormalPath.C_Str());

	// 材质去重
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

	// 创建材质
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
		if (AMesh->mTangents)
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

		FVertex Vertex = FMesh::CreateVertex(Position * Info.ScaleFactor, Normal, TexCoords, Tangent, BiTangent);

		

		Vertices.push_back(Vertex);
	}

	// 处理骨骼
	for(int BoneIndex = 0; BoneIndex < AMesh->mNumBones; BoneIndex++)
	{
		aiBone* ABone = AMesh->mBones[BoneIndex];
		std::string BoneName = ABone->mName.C_Str();

		// 获取偏移矩阵
		aiMatrix4x4 AOffsetMatrix = ABone->mOffsetMatrix;

		// 添加新骨骼
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

		// 处理顶点权重
		int BoneIndexGlobal = Info.BoneMap[BoneName];
		for (unsigned int WeightIndex = 0; WeightIndex < ABone->mNumWeights; WeightIndex++)
		{
			aiVertexWeight AWeight = ABone->mWeights[WeightIndex];
			size_t VertexId = AWeight.mVertexId;
			
			// 找到空闲槽位
			for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
			{
				if (Vertices[VertexId].BoneWeight[j] == 0.0f)
				{
					// --Note: 该Index可能会有错(虽然目前没出错)
					Vertices[VertexId].BoneID[j] = BoneIndexGlobal;
					Vertices[VertexId].BoneWeight[j] = AWeight.mWeight;
					break;
				}
			}
		}
	}

	// 处理索引
	for (unsigned int i = 0; i < AMesh->mNumFaces; i++)
	{
		aiFace Face = AMesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
			Indices.push_back(Face.mIndices[j]);
	}

	// 创建网格
	FMesh* NewMesh = new FMesh(Vertices, Indices);
	Info.Meshs.emplace_back(NewMesh);

	// 获取材质
	aiMaterial* AMaterial = AScene->mMaterials[AMesh->mMaterialIndex];

	// 获取纹理路径
	aiString DiffuseTexPath, SpecularTexPath, NormalPath;

	AMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &DiffuseTexPath);
	AMaterial->GetTexture(aiTextureType_SPECULAR, 0, &SpecularTexPath);
	AMaterial->GetTexture(aiTextureType_NORMALS, 0, &NormalPath);
	std::string DiffusePathStr = Info.ModelDirectory + std::string(DiffuseTexPath.C_Str());
	std::string SpecularPathStr = Info.ModelDirectory + std::string(SpecularTexPath.C_Str());
	std::string NormalPathStr = Info.ModelDirectory + std::string(NormalPath.C_Str());

	// 材质去重
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

	// 创建材质
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

		// PNG、JPG格式
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

			// 复制纹理数据
			unsigned char* TextureData = new unsigned char[TextureSize];
			memcpy(TextureData, ATexture->pcData, TextureSize);

			// 加载纹理到纹理管理器
			MTextureManager::GetInstance().LoadTextureFromMemory(TextureName, TextureData, TextureSize);

			// 释放临时数据
			delete[] TextureData;

			LOG_INFO("Extract embeded texture: %s", TextureName.c_str());
		}
		// TODO:其他格式
		else
		{
			LOG_WARN("Unsupported embeded texture format in model: %s", ModelPath.c_str());
		}
	}
}

void MModelManager::SetBoneHierarchy(aiNode* ANode, FSkeleton& Skeleton, int Depth) const
{
	// 获取当前骨骼
	std::string CurrentNodeName = ANode->mName.C_Str();
	int CurrentBoneIndex = Skeleton.GetIndexOf(CurrentNodeName);
	MySTL::TSharedPtr<FBone> CurrentBonePtr;
	if(CurrentBoneIndex != -1)
		CurrentBonePtr = Skeleton.GetBone(CurrentBoneIndex);

	std::string Black(Depth, '|');
	// LOG_INFO("%s%s, Index: %d", Black.c_str(), CurrentNodeName.c_str(), CurrentBoneIndex);

	// 设置根骨骼
	if (Skeleton.GetRootIndex() < 0 && CurrentBoneIndex >= 0)
	{
		Skeleton.SetRootIndex(CurrentBoneIndex);
		// LOG_INFO("Set root bone: %s", CurrentBonePtr->GetBoneName().c_str());
	}

	// 遍历所有子节点
	int ChildrenCnt = ANode->mNumChildren;
	for (int i = 0; i < ChildrenCnt; i++)
	{
		// 获取子节点骨骼
		std::string ChildrenName = ANode->mChildren[i]->mName.C_Str();
		int ChildrenIndex = Skeleton.GetIndexOf(ChildrenName);

		// 设置父子关系
		if (CurrentBoneIndex != -1 && ChildrenIndex != -1)
		{
			auto ChildPtr = Skeleton.GetBone(ChildrenIndex);
			ChildPtr->SetParent(CurrentBonePtr);
			CurrentBonePtr->AddChild(ChildPtr);
		}

		// 层级递归
		SetBoneHierarchy(ANode->mChildren[i], Skeleton, Depth + 1);
	}
}
		