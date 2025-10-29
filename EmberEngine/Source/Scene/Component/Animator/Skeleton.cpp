// Skeleton.cpp
// ¹Ç÷ÀÀà
// created by FirstRay2001, Oct/22/2025

#include "Skeleton.h"
#include "Bone.h"

FSkeleton::FSkeleton() :
	Bones_(),
	RootIndex_(-1)
{
}

void FSkeleton::Tick(float DeltaTime)
{
	if (RootIndex_ >= 0 && RootIndex_ < Bones_.Size())
	{
		Bones_[RootIndex_]->CalculateFinalTransformRecursively();
	}
}

void FSkeleton::AddBone(const MySTL::TSharedPtr<FBone>& Bone)
{
	Bones_.push_back(Bone);
}

void FSkeleton::AddRootBone(MySTL::TSharedPtr<FBone>& Bone)
{
	if (RootIndex_ != -1)
	{
		Bone->AddChild(Bones_[RootIndex_]);
		Bones_[RootIndex_]->SetParent(Bone);
	}
	Bones_.push_back(Bone);
	RootIndex_ = Bones_.Size() - 1;
}

int FSkeleton::GetRootIndex() const
{
	return RootIndex_;
}

void FSkeleton::SetRootIndex(int NewIndex)
{
	if (NewIndex < 0 || NewIndex >= Bones_.Size())
		return;
	RootIndex_ = NewIndex;
}

MySTL::TSharedPtr<FBone> FSkeleton::GetBone(const std::string& BoneName) const
{
	for (int i = 0; i < Bones_.Size(); i++)
		if (Bones_[i]->GetBoneName() == BoneName)
			return Bones_[i];
	LOG_ERROR("FSkeleton::GetBone: Bone with name: %s not found!", BoneName.c_str());
	return MySTL::TSharedPtr<FBone>(nullptr);
}

MySTL::TSharedPtr<FBone> FSkeleton::GetBone(const std::string& BoneName)
{
	for (int i = 0; i < Bones_.Size(); i++)
		if (Bones_[i]->GetBoneName() == BoneName)
			return Bones_[i];
	LOG_ERROR("FSkeleton::GetBone: Bone with name: %s not found!", BoneName.c_str());
	return MySTL::TSharedPtr<FBone>(nullptr);
}

size_t FSkeleton::GetIndexOf(const std::string& BoneName) const
{
	for (int i = 0; i < Bones_.Size(); i++)
		if (Bones_[i]->GetBoneName() == BoneName)
			return i;
	return -1;
}

MySTL::TSharedPtr<FBone> FSkeleton::GetBone(int Index) const
{
	if (Index >= 0 && Index < Bones_.Size())
		return Bones_[Index];
	LOG_ERROR("FSkeleton::GetBone: Index out of bounds!");
	return MySTL::TSharedPtr<FBone>(nullptr);
}

MySTL::TSharedPtr<FBone> FSkeleton::GetBone(int Index)
{
	if (Index >= 0 && Index < Bones_.Size())
		return Bones_[Index];
	LOG_ERROR("FSkeleton::GetBone: Index out of bounds!");
	return MySTL::TSharedPtr<FBone>(nullptr);
}

MySTL::TVector<MyMath::FMatrix> FSkeleton::GetBoneMatrices() const
{
	MySTL::TVector<MyMath::FMatrix> BoneMatrices;
	for (int i = 0; i < Bones_.Size(); i++)
	{
		MyMath::FMatrix GlobalTransformMatrix = Bones_[i]->GetGlobalTransform();
		MyMath::FMatrix OffsetMatrix = Bones_[i]->GetOffsetMatrix();
		BoneMatrices.push_back(GlobalTransformMatrix * OffsetMatrix);
	}
	return BoneMatrices;
}

void FSkeleton::SetModelMatrix(const MyMath::FMatrix& InModelMatrix)
{
	ModelMatrix_ = InModelMatrix;
	for(int i = 0; i < Bones_.Size(); i ++)
	{
		Bones_[i]->SetModelMatrix(InModelMatrix);
	}
}

void FSkeleton::SetDebugDrawBone(bool bDebug)
{
	for (int i = 0; i < Bones_.Size(); i++)
	{
		Bones_[i]->SetDebugDraw(bDebug);
	}
}

void FSkeleton::APose()
{
	if (RootIndex_ >= 0 && RootIndex_ < Bones_.Size())
	{
		Bones_[RootIndex_]->InitializeAPose();
	}
}
