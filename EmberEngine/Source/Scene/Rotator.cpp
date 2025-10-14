// Rotator.cpp
// ÈýÎ¬Ðý×ª
// created by FirstRay2001, Sep/17/2025

#include "Rotator.h"

FRotator::FRotator() :
    Quaternion_()
{
}

FRotator::FRotator(MyMath::Quaternion Quat) :
	Quaternion_(Quat)
{
}

FRotator::FRotator(float Radian, const MyMath::FVector3& Axis) :
    Quaternion_(Radian, Axis)
{
}

void FRotator::Rotate(MyMath::Quaternion Quat)
{
	Quaternion_ = Quat * Quaternion_;
}

MyMath::FMatrix FRotator::GetMatrix() const
{
    return Quaternion_.ToMatrix();
}

MyMath::Quaternion FRotator::GetQuaternion() const
{
	return Quaternion_;
}

MyMath::FVector3 FRotator::GetForwardVector() const
{
	return Quaternion_.GetForwardDirection();
}

MyMath::FVector3 FRotator::GetRightVector() const
{
	return Quaternion_.GetRightDirection();
}

MyMath::FVector3 FRotator::GetUpVector() const
{
	return Quaternion_.GetUpDirection();
}
