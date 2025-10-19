// PointLightActor.h
// ���Դ��
// created by FirstRay2001, Oct/15/2025

#pragma once

#include "Source/Scene/Actor/Actor.h"

class APointLightActor : public AActor
{
public:
	APointLightActor(MySTL::TSharedPtr<FMesh> Mesh, MySTL::TSharedPtr<FShader> Shader);
	virtual ~APointLightActor() override;

public:
	// ��������ɫ
	void SetAmbientColor(const MyMath::FVector3& LightColor) { AmbientColor_ = LightColor; }
	const MyMath::FVector3& GetAmbientColor() const { return AmbientColor_; }

	// ��������ɫ
	void SetDiffuseColor(const MyMath::FVector3& LightColor) { DiffuseColor_ = LightColor; }
	const MyMath::FVector3& GetDiffuseColor() const { return DiffuseColor_; }

	// ���淴����ɫ
	void SetSpecularColor(const MyMath::FVector3& LightColor) { SpecularColor_ = LightColor; }
	const MyMath::FVector3& GetSpecularColor() const { return SpecularColor_; }

	// ����˥��
	void SetConstant(float Constant) { Constant_ = Constant; }
	float GetConstant() const { return Constant_; }

	// ����˥��
	void SetLinear(float Linear) { Linear_ = Linear; }
	float GetLinear() const { return Linear_; }

	// ����˥��
	void SetQuadratic(float Quadratic) { Quadratic_ = Quadratic; }
	float GetQuadratic() const { return Quadratic_; }


	// ����ģ�;���
	void SetModelMatrix(const char* UniformName = "model") const;

	// �����������
	void SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName = "view") const;

	// ����͸�Ӿ���
	void SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName = "projection") const;

	// ��������
	void SetPointLight() const;

	// �������λ��
	void SetCameraPos(const MyMath::FVector3& CameraPos) const;

	// ���Ƶ��Դ
	void Draw() const;

private:
	MyMath::FVector3			AmbientColor_;
	MyMath::FVector3			DiffuseColor_;
	MyMath::FVector3			SpecularColor_;
	float						Constant_;
	float						Linear_;
	float						Quadratic_;
	MySTL::TSharedPtr<FMesh>	Mesh_;
	MySTL::TWeakPtr<FShader>	Shader_;
};
