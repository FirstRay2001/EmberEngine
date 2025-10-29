// Skeleton.h
// ������
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

	// ��ȡ����������
	int GetRootIndex() const;

	// ���ø���������
	void SetRootIndex(int NewIndex);

	// �������ƻ�ȡ����
	MySTL::TSharedPtr<FBone> GetBone(const std::string& BoneName) const;

	MySTL::TSharedPtr<FBone> GetBone(const std::string& BoneName);

	// �������ƻ�ȡ����
	size_t GetIndexOf(const std::string& BoneName) const;

	// ����������ȡ����
	MySTL::TSharedPtr<FBone> GetBone(int Index) const;

	MySTL::TSharedPtr<FBone> GetBone(int Index);

	MySTL::TVector<MyMath::FMatrix> GetBoneMatrices() const;

	void SetModelMatrix(const MyMath::FMatrix& InModelMatrix);

	void SetDebugDrawBone(bool bDebug);

public:
	// ����ΪA-Pose
	void APose();

private:
	MySTL::TVector<MySTL::TSharedPtr<FBone>> Bones_;
	MyMath::FMatrix ModelMatrix_;
	int RootIndex_;
};
