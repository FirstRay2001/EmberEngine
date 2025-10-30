// Model.h
// 模型类
// created by FirstRay2001, Oct/16/2025

#pragma once

#include "Common.h"

class FModel
{
public:
	FModel();
	FModel(FMesh* Mesh, FMaterial* Material);
	FModel(MySTL::TVector<MySTL::TSharedPtr<FMesh>>& Meshs, const MySTL::TVector<FMaterial>& Materials, const MySTL::TVector<unsigned int>& Indices);
	virtual ~FModel() {};

	// 绘制
	void Draw(const FShader& Shader) const;

	void RawDraw() const;

private:
	MySTL::TVector<MySTL::TSharedPtr<FMesh>>	Meshs_;
	MySTL::TVector<FMaterial>					Materials_;
	MySTL::TVector<unsigned int>				MeshMaterialIndices_;
};
