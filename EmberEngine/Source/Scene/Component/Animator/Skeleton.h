// Skeleton.h
// 骨骼类
// created by FirstRay2001, Oct/22/2025

#pragma once

#include "Common.h"

class FSkeleton
{
public:
	FSkeleton();
	~FSkeleton() {};

public:
	void Tick(float DeltaTime);

public:
	void AddBone(const MySTL::TSharedPtr<FBone>& Bone);

	void AddRootBone(MySTL::TSharedPtr<FBone>& Bone);

	// 获取根骨骼索引
	int GetRootIndex() const;

	// 设置根骨骼索引
	void SetRootIndex(int NewIndex);

	// 根据名称获取骨骼
	MySTL::TSharedPtr<FBone> GetBone(const std::string& BoneName) const;

	MySTL::TSharedPtr<FBone> GetBone(const std::string& BoneName);

	// 根据名称获取索引
	size_t GetIndexOf(const std::string& BoneName) const;

	// 根据索引获取骨骼
	MySTL::TSharedPtr<FBone> GetBone(int Index) const;

	MySTL::TSharedPtr<FBone> GetBone(int Index);

	MySTL::TVector<MyMath::FMatrix> GetBoneMatrices() const;

	void SetModelMatrix(const MyMath::FMatrix& InModelMatrix);

	void SetDebugDrawBone(bool bDebug);

public:
	// 设置为A-Pose
	void APose();

private:
	MySTL::TVector<MySTL::TSharedPtr<FBone>> Bones_;
	MyMath::FMatrix ModelMatrix_;
	int RootIndex_;
};
