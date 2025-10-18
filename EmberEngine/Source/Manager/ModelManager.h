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
	// 加载模型, 传入模型名称，返回模型ID，失败返回-1
	int LoadModel(const std::string& ModelName);

	// 查找模型，传入模型名称，返回模型ID，未找到返回-1
	int FindModel(const std::string& ModelName) const;

	// 获取模型，传入模型ID，返回智能指针
	MySTL::TSharedPtr<FModel> GetModel(int ModelID);

	// 获取模型，传入模型名称，返回智能指针
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
