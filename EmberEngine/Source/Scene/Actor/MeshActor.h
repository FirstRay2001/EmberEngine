// MeshActor.h
// ����Actor��
// created by FirstRay2001, Oct/11/2025

#pragma once

#include "Actor.h"
#include "Source/Graphics/Mesh.h"
#include "Source/Graphics/Shader.h"

class AMeshActor : public AActor
{
public:
	AMeshActor(FMesh& Mesh, FShader& Shader);

public:
	virtual void Tick(float DeltaTime) override;

public:

	// ����ģ�;���
	void SetModelMatrix(const char* UniformName = "model") const;

	// �����������
	void SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName = "view") const;

	// ����͸�Ӿ���
	void SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName = "projection") const;

	// ����������ɫ
	void SetObjectColor(const MyMath::FVector3& ObjectColor, const char* UniformName = "objectColor") const;

	// ���õ��Դλ��
	void SetLightPos(const MyMath::FVector3& LightPos, const char* UniformName = "lightPos") const;

	// ���õ��Դ��ɫ
	void SetLightColor(const MyMath::FVector3& LightColor, const char* UniformName = "lightColor") const;

	// ���������λ��
	void SetCameraPos(const MyMath::FVector3& CameraPos, const char* UniformName = "cameraPos") const;

	// ��������
	void Draw() const;

private:
	FMesh	Mesh_;
	FShader Shader_;
};
