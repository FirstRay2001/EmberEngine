// Transform.h
// 三维变换类
// created by FirstRay2001, Sep/17/2025

#pragma once

#include "Common.h"
#include "Rotator.h"


class FTransform
{
public:
	// 默认构造 无父节点
	FTransform();

	// 有父节点
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


	// 更新绝对变换
	void UpdateAbsolute();

	// 获取世界坐标系下的变换矩阵
	MyMath::FMatrix GetTransformMatrix() const;

	// TODO: 依附、解除依附

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