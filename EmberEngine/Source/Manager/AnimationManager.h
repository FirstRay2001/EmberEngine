// AnimationManager.h
// 动画管理器
// created by FirstRay2001, Oct/27/2025

#pragma once

#include "Common.h"

class MAnimationManager
{
public:
	// 单例
	static MAnimationManager& GetInstance()
	{
		static MAnimationManager Instance;
		return Instance;
	}

public:
	void Initialize();

	void Tick(float DeltaTime);

public:

	void AddAnimation(std::string AnimationName, const MySTL::TSharedPtr<FAnimation>& Animation);

	void AddSkeleton(std::string SkeletonName, const MySTL::TSharedPtr<FSkeleton>& Skeleton);

	MySTL::TSharedPtr<FAnimation> GetAnimation(std::string AnimationName);

	MySTL::TSharedPtr<FSkeleton> GetSkeleton(std::string SkeletonName);

	void PlayAnimation(std::string AnimationName, MySTL::TSharedPtr<FSkeleton> Skeleton);

	void PlayAnimation(MySTL::TSharedPtr<FAnimation>& Animation, MySTL::TSharedPtr<FSkeleton> Skeleton);

	void StopAnimation(std::string AnimationName);

private:
	MAnimationManager() = default;

	MySTL::THashTable<std::string, int>				AnimationMap_;
	MySTL::TVector<MySTL::TSharedPtr<FAnimation>>	Animations_;
	MySTL::TVector<MySTL::TSharedPtr<FAnimation>>	PlayingAnimations_;
	MySTL::THashTable<std::string, int>				SkeletonMap_;
	MySTL::TVector<MySTL::TSharedPtr<FSkeleton>>	Skeletons_;

};
