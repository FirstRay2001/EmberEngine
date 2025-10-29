// Bone.cpp
// 单个骨骼类
// created by FirstRay2001, Oct/22/2025

#include "Bone.h"
#include "Source/Manager/DebugDrawManager.h"

FBone::FBone() :
	LocalTransform_(),
	Parent_()
{
}

FBone::FBone(const MyMath::FMatrix& LocalTransform) :
	LocalTransform_(LocalTransform),
	Parent_()
{
}

FBone::FBone(const MyMath::FMatrix& LocalTransform, MySTL::TSharedPtr<FBone> Parent) :
	LocalTransform_(LocalTransform),
	Parent_(Parent)
{
	SetParent(Parent);
}

void FBone::InitializeAPose()
{
	if (Parent_.IsValid())
	{
		LocalTransform_ = Parent_->GetGlobalTransform().Inverse() * OffsetMatrix_.Inverse();
		GlobalTransform_ = Parent_->GetGlobalTransform() * LocalTransform_;
	}
	else
	{
		LocalTransform_ = OffsetMatrix_.Inverse();
		GlobalTransform_ = LocalTransform_;
	}

	InitialLocal_ = LocalTransform_;

	for (int i = 0; i < Children_.Size(); i++)
	{
		if (Children_[i].IsValid())
			Children_[i]->InitializeAPose();
	}
}

void FBone::CalculateFinalTransformRecursively()
{
	if (Parent_.IsValid())
	{
		GlobalTransform_ = Parent_->GetGlobalTransform() * LocalTransform_;
		
		if (DebugDraw_)
			DebugDrawBone();
	}
	else
	{
		// GlobalTransform_ = MyMath::FMatrix();
		GlobalTransform_ = OffsetMatrix_.Inverse();
		// GlobalTransform_ = LocalTransform_.Inverse();
	}
	for (int i = 0; i < Children_.Size(); i++)
	{
		if (Children_[i].IsValid())
			Children_[i]->CalculateFinalTransformRecursively();
	}
}

void FBone::AddChild(MySTL::TSharedPtr<FBone> Child)
{
	Children_.push_back(Child);
}

bool FBone::RemoveChild(MySTL::TSharedPtr<FBone> Child)
{
	return Children_.Erase(Child);
}

void FBone::DebugDrawBone()
{
	MyMath::FVector4 CurrentPos = ModelMatrix_ * GlobalTransform_ * MyMath::FVector4(0, 0, 0, 1);
	MyMath::FVector4 ParentPos = ModelMatrix_ * Parent_->GetGlobalTransform() * MyMath::FVector4(0, 0, 0, 1);
	MyMath::FVector3 CurrentWorldPos(CurrentPos.X / CurrentPos.W, CurrentPos.Y / CurrentPos.W, CurrentPos.Z / CurrentPos.W);
	MyMath::FVector3 ParentWorldPos(ParentPos.X / ParentPos.W, ParentPos.Y / ParentPos.W, ParentPos.Z / ParentPos.W);
	MDebugDrawManager::GetInstance().AddLine(ParentWorldPos, CurrentWorldPos, MyMath::FVector3(1, 0, 0));
}

void FBone::SetParent(MySTL::TSharedPtr<FBone> Parent)
{
	// 设置父节点
	Parent_ = Parent;
}
