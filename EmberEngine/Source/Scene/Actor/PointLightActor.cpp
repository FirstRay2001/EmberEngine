// PointLightActor.cpp
// 点光源类
// created by FirstRay2001, Oct/15/2025

#include "PointLightActor.h"
#include "Source/Scene/Component/Graphic/Shader.h"
#include "Source/Scene/Component/Graphic/Mesh.h"


APointLightActor::APointLightActor(MySTL::TSharedPtr<FMesh> Mesh, MySTL::TSharedPtr<FShader> Shader) :
	Mesh_(Mesh),
	Shader_(Shader),
	AmbientColor_(MyMath::FVector3(0.2f, 0.2f, 0.2f)),
	DiffuseColor_(MyMath::FVector3(1.0f, 1.0f, 1.0f)),
	SpecularColor_(MyMath::FVector3(1.0f, 1.0f, 1.0f)),
	Constant_(1.0f),
	Linear_(0.09f),
	Quadratic_(0.032f)
{
}

APointLightActor::~APointLightActor()
{
}

void APointLightActor::SetModelMatrix(const char* UniformName) const
{
	// 获取模型矩阵
	MyMath::FMatrix ModelMat = GetWorldMatrix();

	// 转化为列优先存储
	ModelMat.Transpose();

	// 激活Shader
	Shader_->Use();

	// 设置模型矩阵
	Shader_->SetMatrix(UniformName, ModelMat);
}

void APointLightActor::SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName) const
{
	// 转化为列优先存储
	ViewMatrix.Transpose();

	// 激活Shader
	Shader_->Use();

	// 设置相机矩阵
	Shader_->SetMatrix(UniformName, ViewMatrix);
}

void APointLightActor::SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName) const
{
	// 转化为列优先存储
	ProjectionMatrix.Transpose();

	// 激活Shader
	Shader_->Use();

	// 设置透视矩阵
	Shader_->SetMatrix(UniformName, ProjectionMatrix);
}

void APointLightActor::Draw() const
{
	// 激活Shader
	Shader_->Use();

	// 绘制
	Mesh_->Draw();
}
