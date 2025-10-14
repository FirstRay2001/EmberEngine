// Rotator.h
// ÈýÎ¬Ðý×ª
// created by FirstRay2001, Sep/17/2025

#pragma once

#include "Common.h"

class FRotator
{
public:
	FRotator();
	FRotator(MyMath::Quaternion Quat);
	FRotator(float Radian, const MyMath::FVector3& Axis);

public:
	void Rotate(MyMath::Quaternion Quat);

public:
	MyMath::FMatrix GetMatrix() const;

	MyMath::Quaternion GetQuaternion() const;

	MyMath::FVector3 GetForwardVector() const;
	MyMath::FVector3 GetRightVector() const;
	MyMath::FVector3 GetUpVector() const;

private:
	MyMath::Quaternion Quaternion_;
};
