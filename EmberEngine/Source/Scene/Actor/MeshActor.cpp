// MeshActor.cpp
// 网格Actor类
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
	// 旋转测试
	MyMath::Quaternion Rot = ActorTransform_.GetAbsoluteRotation().GetQuaternion();
	MyMath::Quaternion Quat(0.2f * DeltaTime, MyMath::FVector3(0,1,0));

	Rot = Quat * Rot;
	ActorTransform_.SetAbsoluteRotation(Rot);
}

void AMeshActor::SetModelMatrix(const char* UniformName) const
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

void AMeshActor::SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName) const
{
	// 转化为列优先存储
	ViewMatrix.Transpose();

	// 激活Shader
	Shader_->Use();

	// 设置相机矩阵
	Shader_->SetMatrix(UniformName, ViewMatrix);
}

void AMeshActor::SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName) const
{
	// 转化为列优先存储
	ProjectionMatrix.Transpose();

	// 激活Shader
	Shader_->Use();

	// 设置透视矩阵
	Shader_->SetMatrix(UniformName, ProjectionMatrix);
}

void AMeshActor::SetObjectColor(const MyMath::FVector3& ObjectColor, const char* UniformName) const
{
	// 激活Shader
	Shader_->Use();

	// 设置物体颜色
	Shader_->SetVector3(UniformName, ObjectColor);
}

void AMeshActor::SetLightPos(const MyMath::FVector3& LightPos, const char* UniformName) const
{
	// 激活Shader
	Shader_->Use();

	// 设置光源位置
	Shader_->SetVector3(UniformName, LightPos);
}

void AMeshActor::SetLightColor(const MyMath::FVector3& LightColor, const char* UniformName) const
{
	// 激活Shader
	Shader_->Use();

	// 设置光源颜色
	Shader_->SetVector3(UniformName, LightColor);
}

void AMeshActor::SetPointLight(APointLightActor* PointLight)
{
	// 激活Shader
	Shader_->Use();

	// 设置点光源位置
	SetLightPos(PointLight->GetActorWorldLocation(), "pointLight.position");

	// 设置点光源颜色
	SetLightColor(PointLight->GetAmbientColor(), "pointLight.ambient");
	SetLightColor(PointLight->GetDiffuseColor(), "pointLight.diffuse");
	SetLightColor(PointLight->GetSpecularColor(), "pointLight.specular");

	// 设置点光源衰减
	Shader_->SetFloat("pointLight.constant", PointLight->GetConstant());
	Shader_->SetFloat("pointLight.linear", PointLight->GetLinear());
	Shader_->SetFloat("pointLight.quadratic", PointLight->GetQuadratic());
}

void AMeshActor::SetDirectionalLight(ADirectionalLightActor* DirectionalLight)
{
	// 激活Shader
	Shader_->Use();

	// 设置平行光源方向
	Shader_->SetVector3("dirLight.direction", DirectionalLight->GetDirection());

	// 设置平行光源颜色
	SetLightColor(DirectionalLight->GetAmbientColor(), "dirLight.ambient");
	SetLightColor(DirectionalLight->GetDiffuseColor(), "dirLight.diffuse");
	SetLightColor(DirectionalLight->GetSpecularColor(), "dirLight.specular");
}

void AMeshActor::SetCameraPos(const MyMath::FVector3& CameraPos, const char* UniformName) const
{
	// 激活Shader
	Shader_->Use();

	// 设置摄像机位置
	Shader_->SetVector3(UniformName, CameraPos);
}

void AMeshActor::SetTextureSampler(const char* PictureName, const char* UniformName) const
{
	// 激活Shader
	Shader_->Use();

	// 绑定纹理采样器
	MTextureManager::GetInstance().BindSampler(PictureName, *Shader_, UniformName);
}

void AMeshActor::SetMaterial(const FMaterial& Material)
{
	*Material_ = Material;
}

void AMeshActor::Draw() const
{
	// 激活Shader
	Shader_->Use();

	// 应用材质
	Material_->ApplyTo(*Shader_);

	// 绘制
	Mesh_->Draw();
}
