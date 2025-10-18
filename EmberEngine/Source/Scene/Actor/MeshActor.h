// MeshActor.h
// ����Actor��
// created by FirstRay2001, Oct/11/2025

#pragma once

#include "Actor.h"

class AMeshActor : public AActor
{
public:
	AMeshActor(MySTL::TSharedPtr<FMesh>&Mesh, MySTL::TSharedPtr<FShader> Shader, MySTL::TSharedPtr<FMaterial> Material);
	virtual ~AMeshActor() override;

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

	// ���õ��Դ
	void SetPointLight(APointLightActor* PointLight);

	// ����ƽ�й�Դ
	void SetDirectionalLight(ADirectionalLightActor* DirectionalLight);

	// ���������λ��
	void SetCameraPos(const MyMath::FVector3& CameraPos, const char* UniformName = "cameraPos") const;

	// �������������
	void SetTextureSampler(const char* PictureName, const char* UniformName = "texture1") const;

	// ���ò���
	void SetMaterial(const FMaterial& Material);

	// ��������
	void Draw() const;

private:
	MySTL::TSharedPtr<FMesh>		Mesh_;
	MySTL::TWeakPtr<FShader>		Shader_;
	MySTL::TSharedPtr<FMaterial>	Material_;
};
