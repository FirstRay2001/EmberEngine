// Transform.h
// ��ά�任��
// created by FirstRay2001, Sep/17/2025

#pragma once

#include "Common.h"
#include "Rotator.h"


class FTransform
{
public:
	// Ĭ�Ϲ��� �޸��ڵ�
	FTransform();

	// �и��ڵ�
	FTransform(MySTL::TSharedPtr<FTransform>& Parent);

	void SetRelativeLocation(const MyMath::FVector3& NewRelativeLocation);
	void SetRelativeRotation(const FRotator& NewRelativeRotation);
	void SetRelativeScale(const MyMath::FVector3& NewRelativeScale);
	void SetAbsoluteLocation(const MyMath::FVector3& NewAbsoluteLocation);
	void SetAbsoluteRotation(const FRotator& NewAbsoluteRotation);
	void SetAbsoluteScale(const MyMath::FVector3& NewAbsoluteScale);
	MyMath::FVector3 GetAbsoluteLocation() const;
	FRotator GetAbsoluteRotation() const;
	MyMath::FVector3 GetAbsoluteScale() const;

	void Rotate(const MyMath::Quaternion& DeltaRotation);


	// ���¾��Ա任
	void UpdateAbsolute();

	// ��ȡ��������ϵ�µı任����
	MyMath::FMatrix GetTransformMatrix() const;

	// TODO: �������������

private:
	MySTL::TWeakPtr<FTransform>					Parent_;
	MySTL::TVector<MySTL::TWeakPtr<FTransform>>	Children_;
	MyMath::FVector3							AbsoluteLocation_;
	FRotator									AbsoluteRotation_;
	MyMath::FVector3							AbsoluteScale_;
	MyMath::FVector3							RelativeLocation_;
	FRotator									RelativeRotation_;
	MyMath::FVector3							RelativeScale_;
};