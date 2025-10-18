// PointLightActor.cpp
// ���Դ��
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
	// ��ȡģ�;���
	MyMath::FMatrix ModelMat = GetWorldMatrix();

	// ת��Ϊ�����ȴ洢
	ModelMat.Transpose();

	// ����Shader
	Shader_->Use();

	// ����ģ�;���
	Shader_->SetMatrix(UniformName, ModelMat);
}

void APointLightActor::SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName) const
{
	// ת��Ϊ�����ȴ洢
	ViewMatrix.Transpose();

	// ����Shader
	Shader_->Use();

	// �����������
	Shader_->SetMatrix(UniformName, ViewMatrix);
}

void APointLightActor::SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName) const
{
	// ת��Ϊ�����ȴ洢
	ProjectionMatrix.Transpose();

	// ����Shader
	Shader_->Use();

	// ����͸�Ӿ���
	Shader_->SetMatrix(UniformName, ProjectionMatrix);
}

void APointLightActor::Draw() const
{
	// ����Shader
	Shader_->Use();

	// ����
	Mesh_->Draw();
}
