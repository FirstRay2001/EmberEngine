// Transform.cpp
// 三维变换类
// created by FirstRay2001, Sep/17/2025

#include "Transform.h"

FTransform::FTransform() :
	Parent_(),
	Children_(),
	AbsoluteLocation_(0, 0, 0),
	AbsoluteRotation_(),
	AbsoluteScale_(1, 1, 1),
	RelativeLocation_(0, 0, 0),
	RelativeRotation_(),
	RelativeScale_(1, 1, 1)
{
}

FTransform::FTransform(MySTL::TSharedPtr<FTransform>& Parent) :
	Parent_(Parent),
	Children_(),
	AbsoluteLocation_(0,0,0),
	AbsoluteRotation_(),
	AbsoluteScale_(1,1,1),
	RelativeLocation_(0,0,0),
	RelativeRotation_(),
	RelativeScale_(1,1,1)
{
	LOG_INFO("Set Transform Parent");
}

void FTransform::SetRelativeLocation(const MyMath::FVector3& NewRelativeLocation)
{
	RelativeLocation_ = NewRelativeLocation;
}

void FTransform::SetRelativeRotation(const FRotator& NewRelativeRotation)
{
	LOG_INFO("unimplemented");
}

void FTransform::SetRelativeScale(const MyMath::FVector3& NewRelativeScale)
{
	LOG_INFO("unimplemented");
}

void FTransform::SetAbsoluteLocation(const MyMath::FVector3& NewAbsoluteLocation)
{
	AbsoluteLocation_ = NewAbsoluteLocation;
}

void FTransform::SetAbsoluteRotation(const FRotator& NewAbsoluteRotation)
{
	AbsoluteRotation_ = NewAbsoluteRotation;
}

void FTransform::SetAbsoluteScale(const MyMath::FVector3& NewAbsoluteScale)
{
	AbsoluteScale_ = NewAbsoluteScale;
}

MyMath::FVector3 FTransform::GetAbsoluteLocation() const
{
	return AbsoluteLocation_;
}

FRotator FTransform::GetAbsoluteRotation() const
{
	return AbsoluteRotation_;
}

MyMath::FVector3 FTransform::GetAbsoluteScale() const
{
	return AbsoluteScale_;
}

void FTransform::UpdateAbsolute()
{
	// 有父节点
	if (Parent_.IsValid())
	{

	}
	// 无父节点
	else
	{
		AbsoluteLocation_ = RelativeLocation_;
		AbsoluteRotation_ = RelativeRotation_;
		AbsoluteScale_ = RelativeScale_;
	}
}

MyMath::FMatrix FTransform::GetTransformMatrix() const
{
	// 位移
	MyMath::FMatrix TranslationMat = MyMath::ToTranslationMatrix(AbsoluteLocation_);

	// 旋转
	MyMath::FMatrix RotationMat = AbsoluteRotation_.GetMatrix();

	// 缩放
	MyMath::FMatrix ScaleMat = MyMath::ToScaleMatirx(AbsoluteScale_);

	return TranslationMat * RotationMat * ScaleMat;
}

