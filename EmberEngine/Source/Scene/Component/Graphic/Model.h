// Model.h
// ģ����
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

	//// ������Shader���þ���
	//void SetMatrixToShaders(const char* UniformName, const MyMath::FMatrix& Matrix) const;

	//// ������Shader������ά����
	//void SetVector3ToShaders(const char* UniformName, const MyMath::FVector3& Vector) const;

	//// ������Shader���ø�����
	//void SetFloatToShaders(const char* UniformName, float Value) const;

	// ����
	void Draw(const FShader& Shader) const;

	void RawDraw() const;

private:
	MySTL::TVector<FMesh>	Meshs_;
	MySTL::TVector<FMaterial>	Materials_;
	MySTL::TVector<unsigned int>	MeshMaterialIndices_;
};
