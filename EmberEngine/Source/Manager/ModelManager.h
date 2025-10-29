// ModelManager.h
// 模型管理器
// created by FirstRay2001, Oct/16/2025

#pragma once

#include "Common.h"

class aiNode;
class aiScene;
class aiMesh;

class MModelManager
{
public:
	// 单例
	static MModelManager& GetInstance()
	{
		static MModelManager Instance;
		return Instance;
	}

public:
	void Initialize();

public:
	// 加载模型，传入模型名称，返回模型ID，失败返回-1
	int LoadModel(const std::string& ModelName, float ScaleFactor = 1.0f);

	// 加载OBJ模型，传入模型名称，返回模型ID，失败返回-1
	int LoadModelOBJ(const std::string& ModelName, float ScaleFactor);

	// 加载FBX模型，传入模型名称，返回模型ID，失败返回-1
	int LoadModelFBX(const std::string& ModelName, float ScaleFactor);

	// 查找模型，传入模型名称，返回模型ID，未找到返回-1
	int FindModel(const std::string& ModelName) const;

	// 获取模型，传入模型ID，返回智能指针
	MySTL::TSharedPtr<FModel> GetModel(int ModelID);

	// 获取模型，传入模型名称，返回智能指针
	MySTL::TSharedPtr<FModel> GetModel(const std::string& ModelName);

private:
	MModelManager() = default;

	FModel* LoadModelOBJInternal(const char* ModelPath, float ScaleFactor);

	FModel* LoadModelFBXInternal(const char* ModelPath, float ScaleFactor);

	struct FProcessNodeInfo
	{
		MySTL::TVector<FMesh> Meshs;
		MySTL::TVector<FMaterial> Materials;
		MySTL::TVector<unsigned int> MeshMaterialIndices;
		std::string ModelDirectory;
		float ScaleFactor;
		MySTL::TVector<FBone*> Bones;
		MySTL::THashTable<std::string, size_t> BoneMap;
	};

	void ProcessNodeOBJ(aiNode* ANode, const aiScene* AScene, FProcessNodeInfo& Info);

	void ProcessNodeFBX(aiNode* ANode, const aiScene* AScene, FProcessNodeInfo& Info);

	void ProcessMeshOBJ(aiMesh* AMesh, const aiScene* AScene, FProcessNodeInfo& Info);

	void ProcessMeshFBX(aiMesh* AMesh, const aiScene* AScene, FProcessNodeInfo& Info);

	void ProcessEmbededTextures(const aiScene* AScene, std::string ModelPath);

	// 设置骨骼层级关系
	void SetBoneHierarchy(aiNode* ANode, FSkeleton& Skeleton, int Depth = 0) const;

private:
	MySTL::THashTable<std::string, int>			ModelMap_;
	MySTL::TVector<MySTL::TSharedPtr<FModel>>	Models_;
};
