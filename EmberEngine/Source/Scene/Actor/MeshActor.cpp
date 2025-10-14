// MeshActor.cpp
// 网格Actor类
// created by FirstRay2001, Oct/11/2025

#include "MeshActor.h"

AMeshActor::AMeshActor(FMesh& Mesh, FShader& Shader) :
	Mesh_(Mesh),
	Shader_(Shader)
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
	Shader_.Use();

	// 设置模型矩阵
	Shader_.SetMatrix(UniformName, ModelMat);
}

void AMeshActor::SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName) const
{
	// 转化为列优先存储
	ViewMatrix.Transpose();

	// 激活Shader
	Shader_.Use();

	// 设置相机矩阵
	Shader_.SetMatrix(UniformName, ViewMatrix);
}

void AMeshActor::SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName) const
{
	// 转化为列优先存储
	ProjectionMatrix.Transpose();

	// 激活Shader
	Shader_.Use();

	// 设置透视矩阵
	Shader_.SetMatrix(UniformName, ProjectionMatrix);
}

void AMeshActor::SetObjectColor(const MyMath::FVector3& ObjectColor, const char* UniformName) const
{
	// 激活Shader
	Shader_.Use();

	// 设置物体颜色
	Shader_.SetVector3(UniformName, ObjectColor);
}

void AMeshActor::SetLightPos(const MyMath::FVector3& LightPos, const char* UniformName) const
{
	// 激活Shader
	Shader_.Use();

	// 设置光源位置
	Shader_.SetVector3(UniformName, LightPos);
}

void AMeshActor::SetLightColor(const MyMath::FVector3& LightColor, const char* UniformName) const
{
	// 激活Shader
	Shader_.Use();

	// 设置光源颜色
	Shader_.SetVector3(UniformName, LightColor);
}

void AMeshActor::SetCameraPos(const MyMath::FVector3& CameraPos, const char* UniformName) const
{
	// 激活Shader
	Shader_.Use();

	// 设置摄像机位置
	Shader_.SetVector3(UniformName, CameraPos);
}

void AMeshActor::Draw() const
{
	Shader_.Use();

	// 绘制
	Mesh_.Draw();
}
