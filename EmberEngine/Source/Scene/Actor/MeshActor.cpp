// MeshActor.cpp
// ����Actor��
// created by FirstRay2001, Oct/11/2025

#include "MeshActor.h"
#include "Source/Scene/Actor/PointLightActor.h"
#include "Source/Scene/Actor/DirectionalLightActor.h"
#include "Source/Manager/TextureManager.h"
#include "Source/Scene/Component/Graphic/Mesh.h"
#include "Source/Scene/Component/Graphic/Shader.h"
#include "Source/Scene/Component/Graphic/Material.h"

AMeshActor::AMeshActor(MySTL::TSharedPtr<FMesh>& Mesh, MySTL::TSharedPtr<FShader> Shader, MySTL::TSharedPtr<FMaterial> Material) :
	Mesh_(Mesh),
	Shader_(Shader),
	Material_(Material)
{
}

AMeshActor::~AMeshActor()
{
}

void AMeshActor::Tick(float DeltaTime)
{
	// ��ת����
	MyMath::Quaternion Rot = ActorTransform_.GetAbsoluteRotation().GetQuaternion();
	MyMath::Quaternion Quat(0.2f * DeltaTime, MyMath::FVector3(0,1,0));

	Rot = Quat * Rot;
	ActorTransform_.SetAbsoluteRotation(Rot);
}

void AMeshActor::SetModelMatrix(const char* UniformName) const
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

void AMeshActor::SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName) const
{
	// ת��Ϊ�����ȴ洢
	ViewMatrix.Transpose();

	// ����Shader
	Shader_->Use();

	// �����������
	Shader_->SetMatrix(UniformName, ViewMatrix);
}

void AMeshActor::SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName) const
{
	// ת��Ϊ�����ȴ洢
	ProjectionMatrix.Transpose();

	// ����Shader
	Shader_->Use();

	// ����͸�Ӿ���
	Shader_->SetMatrix(UniformName, ProjectionMatrix);
}

void AMeshActor::SetObjectColor(const MyMath::FVector3& ObjectColor, const char* UniformName) const
{
	// ����Shader
	Shader_->Use();

	// ����������ɫ
	Shader_->SetVector3(UniformName, ObjectColor);
}

void AMeshActor::SetLightPos(const MyMath::FVector3& LightPos, const char* UniformName) const
{
	// ����Shader
	Shader_->Use();

	// ���ù�Դλ��
	Shader_->SetVector3(UniformName, LightPos);
}

void AMeshActor::SetLightColor(const MyMath::FVector3& LightColor, const char* UniformName) const
{
	// ����Shader
	Shader_->Use();

	// ���ù�Դ��ɫ
	Shader_->SetVector3(UniformName, LightColor);
}

void AMeshActor::SetPointLight(APointLightActor* PointLight)
{
	// ����Shader
	Shader_->Use();

	// ���õ��Դλ��
	SetLightPos(PointLight->GetActorWorldLocation(), "pointLight.position");

	// ���õ��Դ��ɫ
	SetLightColor(PointLight->GetAmbientColor(), "pointLight.ambient");
	SetLightColor(PointLight->GetDiffuseColor(), "pointLight.diffuse");
	SetLightColor(PointLight->GetSpecularColor(), "pointLight.specular");

	// ���õ��Դ˥��
	Shader_->SetFloat("pointLight.constant", PointLight->GetConstant());
	Shader_->SetFloat("pointLight.linear", PointLight->GetLinear());
	Shader_->SetFloat("pointLight.quadratic", PointLight->GetQuadratic());
}

void AMeshActor::SetDirectionalLight(ADirectionalLightActor* DirectionalLight)
{
	// ����Shader
	Shader_->Use();

	// ����ƽ�й�Դ����
	Shader_->SetVector3("dirLight.direction", DirectionalLight->GetDirection());

	// ����ƽ�й�Դ��ɫ
	SetLightColor(DirectionalLight->GetAmbientColor(), "dirLight.ambient");
	SetLightColor(DirectionalLight->GetDiffuseColor(), "dirLight.diffuse");
	SetLightColor(DirectionalLight->GetSpecularColor(), "dirLight.specular");
}

void AMeshActor::SetCameraPos(const MyMath::FVector3& CameraPos, const char* UniformName) const
{
	// ����Shader
	Shader_->Use();

	// ���������λ��
	Shader_->SetVector3(UniformName, CameraPos);
}

void AMeshActor::SetTextureSampler(const char* PictureName, const char* UniformName) const
{
	// ����Shader
	Shader_->Use();

	// �����������
	MTextureManager::GetInstance().BindSampler(PictureName, *Shader_, UniformName);
}

void AMeshActor::SetMaterial(const FMaterial& Material)
{
	*Material_ = Material;
}

void AMeshActor::Draw() const
{
	// ����Shader
	Shader_->Use();

	// Ӧ�ò���
	Material_->ApplyTo(*Shader_);

	// ����
	Mesh_->Draw();
}
