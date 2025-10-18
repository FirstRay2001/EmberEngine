// MeshActor.h
// 网格Actor类
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
	// 设置模型矩阵
	void SetModelMatrix(const char* UniformName = "model") const;

	// 设置相机矩阵
	void SetViewMatrix(MyMath::FMatrix ViewMatrix, const char* UniformName = "view") const;

	// 设置透视矩阵
	void SetProjectionMatrix(MyMath::FMatrix ProjectionMatrix, const char* UniformName = "projection") const;

	// 设置物体颜色
	void SetObjectColor(const MyMath::FVector3& ObjectColor, const char* UniformName = "objectColor") const;

	// 设置点光源位置
	void SetLightPos(const MyMath::FVector3& LightPos, const char* UniformName = "lightPos") const;

	// 设置点光源颜色
	void SetLightColor(const MyMath::FVector3& LightColor, const char* UniformName = "lightColor") const;

	// 设置点光源
	void SetPointLight(APointLightActor* PointLight);

	// 设置平行光源
	void SetDirectionalLight(ADirectionalLightActor* DirectionalLight);

	// 设置摄像机位置
	void SetCameraPos(const MyMath::FVector3& CameraPos, const char* UniformName = "cameraPos") const;

	// 设置纹理采样器
	void SetTextureSampler(const char* PictureName, const char* UniformName = "texture1") const;

	// 设置材质
	void SetMaterial(const FMaterial& Material);

	// 绘制网格
	void Draw() const;

private:
	MySTL::TSharedPtr<FMesh>		Mesh_;
	MySTL::TWeakPtr<FShader>		Shader_;
	MySTL::TSharedPtr<FMaterial>	Material_;
};
