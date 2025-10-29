// ModelActor.cpp
// 模型Actor类
// created by FirstRay2001, Oct/16/2025

#include "ModelActor.h"
#include "CameraActor.h"
#include "PointLightActor.h"
#include "DirectionalLightActor.h"
#include "Source/Scene/Component/Graphic/Model.h"
#include "Source/Scene/Component/Graphic/Mesh.h"
#include "Source/Scene/Component/Graphic/Material.h"
#include "Source/Scene/Component/Graphic/Shader.h"
#include "Source/Scene/Component/Animator/Skeleton.h"

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

		// 设置相机位置
		Shader_->SetVector3("cameraPos", Camera.GetActorWorldLocation());

		// 设置VP矩阵
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

		// 设置点光源位置
		Shader_->SetVector3("pointLight.position", PointLight.GetActorWorldLocation());

		// 设置点光源颜色
		Shader_->SetVector3("pointLight.ambient", PointLight.GetAmbientColor());
		Shader_->SetVector3("pointLight.diffuse", PointLight.GetDiffuseColor());
		Shader_->SetVector3("pointLight.specular", PointLight.GetSpecularColor());

		// 设置点光源衰减
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

		// 设置平行光方向
		Shader_->SetVector3("dirLight.direction", DirectionalLight.GetDirection());

		// 设置平行光颜色
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

void AModelActor::SetBoneMatricesToShader(const MySTL::TWeakPtr<FShader>& InShader) const
{
	// 检测有效性
	IfFailGo(Skeleton_.IsValid() && InShader.IsValid());

	// 激活Shader
	InShader->Use();

	// 设置骨骼动画矩阵
	const MySTL::TVector<MyMath::FMatrix>& BoneMatrices = Skeleton_->GetBoneMatrices();
	for (size_t i = 0; i < BoneMatrices.Size(); i++)
	{
		std::string UniformName = "finalBonesMatrices[" + std::to_string(i) + "]";
		InShader->SetMatrix(UniformName.c_str(), BoneMatrices[i].TransposeConst());
	}
}

void AModelActor::Draw() const
{
	if (Model_.IsValid() && Shader_.IsValid())
	{
		// 激活Shader
		Shader_->Use();

		// 设置模型矩阵
		Shader_->SetMatrix("model", GetWorldMatrix().Transpose());

		// 设置骨骼矩阵
		SetBoneMatricesToShader(Shader_);

		// 绘制模型
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

void AModelActor::SetDrawBone(bool bDebug)
{
	if (Skeleton_.IsValid())
		Skeleton_->SetDebugDrawBone(bDebug);
}

void AModelActor::SetSkeleton(MySTL::TSharedPtr<FSkeleton> NewSkeleton)
{
	Skeleton_ = NewSkeleton;
	Skeleton_->SetModelMatrix(GetWorldMatrix());
}
