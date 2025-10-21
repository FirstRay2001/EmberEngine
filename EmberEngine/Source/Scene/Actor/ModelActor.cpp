// ModelActor.cpp
// ģ��Actor��
// created by FirstRay2001, Oct/16/2025

#include "ModelActor.h"
#include "CameraActor.h"
#include "PointLightActor.h"
#include "DirectionalLightActor.h"
#include "Source/Scene/Component/Graphic/Model.h"
#include "Source/Scene/Component/Graphic/Mesh.h"
#include "Source/Scene/Component/Graphic/Material.h"
#include "Source/Scene/Component/Graphic/Shader.h"

AModelActor::AModelActor(MySTL::TSharedPtr<FModel> Model, MySTL::TSharedPtr<FShader> Shader) :
	Model_(Model),
	Shader_(Shader),
	bDrawOutline_(false),
	bDrawNormal_(false)
{
}

AModelActor::AModelActor(FMesh* Mesh, FMaterial* Material, FShader* Shader) :
	Model_(MySTL::TSharedPtr<FModel>(new FModel(Mesh, Material))),
	Shader_(MySTL::TSharedPtr<FShader>(Shader)),
	bDrawOutline_(false),
	bDrawNormal_(false)
{
}

AModelActor::~AModelActor()
{
}

void AModelActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

void AModelActor::SetCamera(const ACameraActor& Camera) const
{
	if (Shader_.IsValid())
	{
		Shader_->Use();

		// �������λ��
		Shader_->SetVector3("cameraPos", Camera.GetActorWorldLocation());

		// ����VP����
		MyMath::FMatrix ViewMatrix = Camera.GetWorldMatrix().Inverse().Transpose();
		MyMath::FMatrix ProjectionMatrix = Camera.GetProjectionMatrix().Transpose();
		Shader_->SetMatrix("view", ViewMatrix);
		Shader_->SetMatrix("projection", ProjectionMatrix);
	}
}

void AModelActor::SetPointLight(const APointLightActor& PointLight) const
{
	if (Shader_.IsValid())
	{
		Shader_->Use();

		// ���õ��Դλ��
		Shader_->SetVector3("pointLight.position", PointLight.GetActorWorldLocation());

		// ���õ��Դ��ɫ
		Shader_->SetVector3("pointLight.ambient", PointLight.GetAmbientColor());
		Shader_->SetVector3("pointLight.diffuse", PointLight.GetDiffuseColor());
		Shader_->SetVector3("pointLight.specular", PointLight.GetSpecularColor());

		// ���õ��Դ˥��
		Shader_->SetFloat("pointLight.constant", PointLight.GetConstant());
		Shader_->SetFloat("pointLight.linear", PointLight.GetLinear());
		Shader_->SetFloat("pointLight.quadratic", PointLight.GetQuadratic());
	}
}

void AModelActor::SetDirectionalLight(const ADirectionalLightActor& DirectionalLight) const
{
	if (Shader_.IsValid())
	{
		Shader_->Use();

		// ����ƽ�йⷽ��
		Shader_->SetVector3("dirLight.direction", DirectionalLight.GetDirection());

		// ����ƽ�й���ɫ
		Shader_->SetVector3("dirLight.ambient", DirectionalLight.GetAmbientColor());
		Shader_->SetVector3("dirLight.diffuse", DirectionalLight.GetDiffuseColor());
		Shader_->SetVector3("dirLight.specular", DirectionalLight.GetSpecularColor());
	}
}

void AModelActor::SetLightSpaceMatrix(const MyMath::FMatrix& LightSpacecMatrix) const
{
	if (Shader_.IsValid())
	{
		Shader_->Use();

		Shader_->SetMatrix("lightSpaceMatrix", LightSpacecMatrix.TransposeConst());
	}
}

void AModelActor::Draw() const
{
	if (Model_.IsValid() && Shader_.IsValid())
	{
		Shader_->SetMatrix("model", GetWorldMatrix().Transpose());
		Model_->Draw(*Shader_);
	}
}

void AModelActor::RawDraw() const
{
	if (Model_.IsValid())
	{
		Model_->RawDraw();
	}
}

const FShader* AModelActor::GetShader() const
{
	return Shader_.Get();
}
