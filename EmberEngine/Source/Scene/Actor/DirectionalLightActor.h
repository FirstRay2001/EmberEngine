// DirectionalLightActor.h
// 平行光源类
// created by FirstRay2001, Oct/15/2025

#pragma once
#include "Source/Scene/Actor/Actor.h"

class ADirectionalLightActor : public AActor
{
public:
	ADirectionalLightActor(const MyMath::FVector3& Direction = MyMath::FVector3(0, -1, 0));

public:
	// 方向
	void SetDirection(const MyMath::FVector3& Direction) { Direction_ = Direction.Normalized(); }
	const MyMath::FVector3& GetDirection() const { return Direction_; }

	// 环境光颜色
	void SetAmbientColor(const MyMath::FVector3& LightColor) { AmbientColor_ = LightColor; }
	const MyMath::FVector3& GetAmbientColor() const { return AmbientColor_; }

	// 漫反射颜色
	void SetDiffuseColor(const MyMath::FVector3& LightColor) { DiffuseColor_ = LightColor; }
	const MyMath::FVector3& GetDiffuseColor() const { return DiffuseColor_; }

	// 镜面反射颜色
	void SetSpecularColor(const MyMath::FVector3& LightColor) { SpecularColor_ = LightColor; }
	const MyMath::FVector3& GetSpecularColor() const { return SpecularColor_; }

private:
	MyMath::FVector3 Direction_;
	MyMath::FVector3 AmbientColor_;
	MyMath::FVector3 DiffuseColor_;
	MyMath::FVector3 SpecularColor_;
};