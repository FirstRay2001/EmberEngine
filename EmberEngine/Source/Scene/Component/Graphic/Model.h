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
	FModel(const MySTL::TVector<FMesh>& Meshs, const MySTL::TVector<FMaterial>& Materials, const MySTL::TVector<unsigned int>& Indices);
	virtual ~FModel() {};

	//// 给所有Shader设置矩阵
	//void SetMatrixToShaders(const char* UniformName, const MyMath::FMatrix& Matrix) const;

	//// 给所有Shader设置三维向量
	//void SetVector3ToShaders(const char* UniformName, const MyMath::FVector3& Vector) const;

	//// 给所有Shader设置浮点数
	//void SetFloatToShaders(const char* UniformName, float Value) const;

	// 绘制
	void Draw(const FShader& Shader) const;

	void RawDraw() const;

private:
	MySTL::TVector<FMesh>	Meshs_;
	MySTL::TVector<FMaterial>	Materials_;
	MySTL::TVector<unsigned int>	MeshMaterialIndices_;
};
