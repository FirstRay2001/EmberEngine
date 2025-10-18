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
	// ����ģ��, ����ģ�����ƣ�����ģ��ID��ʧ�ܷ���-1
	int LoadModel(const std::string& ModelName);

	// ����ģ�ͣ�����ģ�����ƣ�����ģ��ID��δ�ҵ�����-1
	int FindModel(const std::string& ModelName) const;

	// ��ȡģ�ͣ�����ģ��ID����������ָ��
	MySTL::TSharedPtr<FModel> GetModel(int ModelID);

	// ��ȡģ�ͣ�����ģ�����ƣ���������ָ��
	MySTL::TSharedPtr<FModel> GetModel(const std::string& ModelName);

private:
	class FModelUnit;

	MModelManager() = default;

	FModel* LoadModelInternal(const char* ModelPath);

	void ProcessNode(aiNode* ANode, const aiScene* AScene, MySTL::TVector<FMesh>& Meshs, MySTL::TVector<FMaterial>& Materials, MySTL::TVector<unsigned int>& MeshMaterialIndices, std::string ModelDirectory);

	void ProcessMesh(aiMesh* AMesh, const aiScene* AScene, MySTL::TVector<FMesh>& Meshs, MySTL::TVector<FMaterial>& Materials, MySTL::TVector<unsigned int>& MeshMaterialIndices, std::string ModelDirectory);

private:
	MySTL::THashTable<std::string, int>			ModelMap_;
	MySTL::TVector<MySTL::TSharedPtr<FModel>>	Models_;
};
