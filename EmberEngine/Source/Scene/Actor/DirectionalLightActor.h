// DirectionalLightActor.h
// ƽ�й�Դ��
// created by FirstRay2001, Oct/15/2025

#pragma once
#include "Source/Scene/Actor/Actor.h"

class ADirectionalLightActor : public AActor
{
public:
	ADirectionalLightActor(const MyMath::FVector3& Direction = MyMath::FVector3(0, -1, 0));

public:
	// ����
	void SetDirection(const MyMath::FVector3& Direction) { Direction_ = Direction.Normalized(); }
	const MyMath::FVector3& GetDirection() const { return Direction_; }

	// ��������ɫ
	void SetAmbientColor(const MyMath::FVector3& LightColor) { AmbientColor_ = LightColor; }
	const MyMath::FVector3& GetAmbientColor() const { return AmbientColor_; }

	// ��������ɫ
	void SetDiffuseColor(const MyMath::FVector3& LightColor) { DiffuseColor_ = LightColor; }
	const MyMath::FVector3& GetDiffuseColor() const { return DiffuseColor_; }

	// ���淴����ɫ
	void SetSpecularColor(const MyMath::FVector3& LightColor) { SpecularColor_ = LightColor; }
	const MyMath::FVector3& GetSpecularColor() const { return SpecularColor_; }

private:
	MyMath::FVector3 Direction_;
	MyMath::FVector3 AmbientColor_;
	MyMath::FVector3 DiffuseColor_;
	MyMath::FVector3 SpecularColor_;
};