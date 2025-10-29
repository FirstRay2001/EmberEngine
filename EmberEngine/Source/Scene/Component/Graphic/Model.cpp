// Model.cpp
// 模型类
// created by FirstRay2001, Oct/16/2025

#include "Model.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Source/Scene/Component/Animator/Skeleton.h"

FModel::FModel()
{
}

FModel::FModel(FMesh* Mesh, FMaterial* Material)
{
	Meshs_.push_back(*Mesh);
	Materials_.push_back(*Material);
	MeshMaterialIndices_.push_back(0);
}

FModel::FModel(const MySTL::TVector<FMesh>& Meshs, const MySTL::TVector<FMaterial>& Materials, const MySTL::TVector<unsigned int>& Indices) :
	Meshs_(Meshs),
	Materials_(Materials),
	MeshMaterialIndices_(Indices)
{
}

void FModel::Draw(const FShader& Shader) const
{
	for (int i = 0; i < Meshs_.Size(); i++)
	{
		// 应用骨骼动画
		if (Skeleton_.IsValid())
		{

		}

		FMesh Mesh = Meshs_[i];
		if (Materials_.Size() > 0)
		{
			FMaterial Material = Materials_[MeshMaterialIndices_[i]];
			Material.ApplyTo(Shader);
		}
		Mesh.Draw();
	}
}

void FModel::RawDraw() const
{
	for (int i = 0; i < Meshs_.Size(); i++)
	{
		FMesh Mesh = Meshs_[i];
		Mesh.Draw();
	}
}
