// Bone.h
// 单个骨骼类
// created by FirstRay2001, Oct/22/2025

#pragma once

#include "Common.h" 

class FBone
{
public:
	FBone();
	FBone(const MyMath::FMatrix& LocalTransform);
	FBone(const MyMath::FMatrix& LocalTransform, MySTL::TSharedPtr<FBone> Parent);

public:
	// 初始化A-Pose
	void InitializeAPose();

	// 递归更新骨骼
	void CalculateFinalTransformRecursively();
	
public:
	const MyMath::FMatrix GetLocalTransform() const					{ return LocalTransform_; }
	void SetLocalTransform(const MyMath::FMatrix& InLocalTransform) { LocalTransform_ = InLocalTransform; }
	const MyMath::FMatrix GetGlobalTransform() const				{ return GlobalTransform_; }
	std::string GetBoneName() const									{ return BoneName_; }
	void SetBoneName(const std::string& Name)						{ BoneName_ = Name; }
	const MyMath::FMatrix GetOffsetMatrix() const					{ return OffsetMatrix_; }
	void SetOffsetMatrix(const MyMath::FMatrix& InOffsetMatrix)		{ OffsetMatrix_ = InOffsetMatrix; }
	const MyMath::FMatrix GetInitialLocal() const					{ return InitialLocal_; }
	void SetModelMatrix(const MyMath::FMatrix& InModelMatrix)		{ ModelMatrix_ = InModelMatrix; }
	void SetDebugDraw(bool bDebug) 									{ DebugDraw_ = bDebug; }

	void SetParent(MySTL::TSharedPtr<FBone> Parent);

	void AddChild(MySTL::TSharedPtr<FBone> Child);

private:
	bool RemoveChild(MySTL::TSharedPtr<FBone> Child);

	void DebugDrawBone();

private:
	std::string									BoneName_;
	MyMath::FMatrix								LocalTransform_;
	MyMath::FMatrix								GlobalTransform_;
	MyMath::FMatrix								InitialLocal_;
	MyMath::FMatrix								OffsetMatrix_;
	MySTL::TWeakPtr<FBone>						Parent_;
	MySTL::TVector<MySTL::TSharedPtr<FBone>>	Children_;
	MyMath::FMatrix								ModelMatrix_;
	bool DebugDraw_ = false;;
};
