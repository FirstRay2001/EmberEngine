// ModelManager.h
// ģ�͹�����
// created by FirstRay2001, Oct/16/2025

#pragma once

#include "Common.h"

class aiNode;
class aiScene;
class aiMesh;

class MModelManager
{
public:
	// ����
	static MModelManager& GetInstance()
	{
		static MModelManager Instance;
		return Instance;
	}

public:
	void Initialize();

public:
	// ����ģ�ͣ�����ģ�����ƣ�����ģ��ID��ʧ�ܷ���-1
	int LoadModel(const std::string& ModelName, float ScaleFactor = 1.0f);

	// ����OBJģ�ͣ�����ģ�����ƣ�����ģ��ID��ʧ�ܷ���-1
	int LoadModelOBJ(const std::string& ModelName, float ScaleFactor);

	// ����FBXģ�ͣ�����ģ�����ƣ�����ģ��ID��ʧ�ܷ���-1
	int LoadModelFBX(const std::string& ModelName, float ScaleFactor);

	// ����ģ�ͣ�����ģ�����ƣ�����ģ��ID��δ�ҵ�����-1
	int FindModel(const std::string& ModelName) const;

	// ��ȡģ�ͣ�����ģ��ID����������ָ��
	MySTL::TSharedPtr<FModel> GetModel(int ModelID);

	// ��ȡģ�ͣ�����ģ�����ƣ���������ָ��
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

	// ���ù����㼶��ϵ
	void SetBoneHierarchy(aiNode* ANode, FSkeleton& Skeleton, int Depth = 0) const;

private:
	MySTL::THashTable<std::string, int>			ModelMap_;
	MySTL::TVector<MySTL::TSharedPtr<FModel>>	Models_;
};
