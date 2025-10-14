// MeshActor.cpp
// ����Actor��
// created by FirstRay2001, Oct/11/2025

#include "MeshActor.h"

AMeshActor::AMeshActor(FMesh& Mesh, FShader& Shader) :
	Mesh_(Mesh),
	Shader_(Shader)
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
	Shader_.Use();

	// ����ģ�;���
	Shader_.SetMatrix(UniformName, ModelMat);
}

void AMeshActor::SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName) const
{
	// ת��Ϊ�����ȴ洢
	ViewMatrix.Transpose();

	// ����Shader
	Shader_.Use();

	// �����������
	Shader_.SetMatrix(UniformName, ViewMatrix);
}

void AMeshActor::SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName) const
{
	// ת��Ϊ�����ȴ洢
	ProjectionMatrix.Transpose();

	// ����Shader
	Shader_.Use();

	// ����͸�Ӿ���
	Shader_.SetMatrix(UniformName, ProjectionMatrix);
}

void AMeshActor::SetObjectColor(const MyMath::FVector3& ObjectColor, const char* UniformName) const
{
	// ����Shader
	Shader_.Use();

	// ����������ɫ
	Shader_.SetVector3(UniformName, ObjectColor);
}

void AMeshActor::SetLightPos(const MyMath::FVector3& LightPos, const char* UniformName) const
{
	// ����Shader
	Shader_.Use();

	// ���ù�Դλ��
	Shader_.SetVector3(UniformName, LightPos);
}

void AMeshActor::SetLightColor(const MyMath::FVector3& LightColor, const char* UniformName) const
{
	// ����Shader
	Shader_.Use();

	// ���ù�Դ��ɫ
	Shader_.SetVector3(UniformName, LightColor);
}

void AMeshActor::SetCameraPos(const MyMath::FVector3& CameraPos, const char* UniformName) const
{
	// ����Shader
	Shader_.Use();

	// ���������λ��
	Shader_.SetVector3(UniformName, CameraPos);
}

void AMeshActor::Draw() const
{
	Shader_.Use();

	// ����
	Mesh_.Draw();
}
