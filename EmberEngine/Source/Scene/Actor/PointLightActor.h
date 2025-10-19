// PointLightActor.h
// 点光源类
// created by FirstRay2001, Oct/15/2025

#pragma once

#include "Source/Scene/Actor/Actor.h"

class APointLightActor : public AActor
{
public:
	APointLightActor(MySTL::TSharedPtr<FMesh> Mesh, MySTL::TSharedPtr<FShader> Shader);
	virtual ~APointLightActor() override;

public:
	// 环境光颜色
	void SetAmbientColor(const MyMath::FVector3& LightColor) { AmbientColor_ = LightColor; }
	const MyMath::FVector3& GetAmbientColor() const { return AmbientColor_; }

	// 漫反射颜色
	void SetDiffuseColor(const MyMath::FVector3& LightColor) { DiffuseColor_ = LightColor; }
	const MyMath::FVector3& GetDiffuseColor() const { return DiffuseColor_; }

	// 镜面反射颜色
	void SetSpecularColor(const MyMath::FVector3& LightColor) { SpecularColor_ = LightColor; }
	const MyMath::FVector3& GetSpecularColor() const { return SpecularColor_; }

	// 常数衰减
	void SetConstant(float Constant) { Constant_ = Constant; }
	float GetConstant() const { return Constant_; }

	// 线性衰减
	void SetLinear(float Linear) { Linear_ = Linear; }
	float GetLinear() const { return Linear_; }

	// 二次衰减
	void SetQuadratic(float Quadratic) { Quadratic_ = Quadratic; }
	float GetQuadratic() const { return Quadratic_; }


	// 设置模型矩阵
	void SetModelMatrix(const char* UniformName = "model") const;

	// 设置相机矩阵
	void SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName = "view") const;

	// 设置透视矩阵
	void SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName = "projection") const;

	// 设置自身
	void SetPointLight() const;

	// 设置相机位置
	void SetCameraPos(const MyMath::FVector3& CameraPos) const;

	// 绘制点光源
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
