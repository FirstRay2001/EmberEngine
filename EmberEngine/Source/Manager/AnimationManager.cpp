// AnimationManager.cpp
// ∂Øª≠π‹¿Ì∆˜
// created by FirstRay2001, Oct/27/2025

#include "AnimationManager.h"
#include "Source/Scene/Component/Animator/Skeleton.h"
#include "Source/Scene/Component/Animator/Bone.h"
#include "Source/Scene/Component/Animator/Animation.h"

void MAnimationManager::Initialize()
{
}

void MAnimationManager::Tick(float DeltaTime)
{
	for (int i = 0; i < PlayingAnimations_.Size(); i++)
	{
		if (PlayingAnimations_[i].IsValid())
		{
			PlayingAnimations_[i]->Tick(DeltaTime);
		}
	}
}

void MAnimationManager::AddAnimation(std::string AnimationName, const MySTL::TSharedPtr<FAnimation>& Animation)
{
	if (AnimationMap_.Find(AnimationName) != nullptr)
	{
		LOG_WARN("Animation already exists: %s", AnimationName.c_str());
		return;
	}

	AnimationMap_[AnimationName] = Animations_.Size();
	Animations_.push_back(Animation);
}

void MAnimationManager::AddSkeleton(std::string SkeletonName, const MySTL::TSharedPtr<FSkeleton>& Skeleton)
{
	if (SkeletonMap_.Find(SkeletonName) != nullptr)
	{
		LOG_WARN("Skeleton already exists: %s", SkeletonName.c_str());
		return;
	}

	SkeletonMap_[SkeletonName] = Skeletons_.Size();
	Skeletons_.push_back(Skeleton);
}

MySTL::TSharedPtr<FAnimation> MAnimationManager::GetAnimation(std::string AnimationName)
{
	if (AnimationMap_.Find(AnimationName) != nullptr)
	{
		return Animations_[AnimationMap_[AnimationName]];
	}
	else
	{
		LOG_ERROR("Animation not found: %s", AnimationName.c_str());
		return MySTL::TSharedPtr<FAnimation>();
	}
}

MySTL::TSharedPtr<FSkeleton> MAnimationManager::GetSkeleton(std::string SkeletonName)
{
	if (SkeletonMap_.Find(SkeletonName) != nullptr)
	{
		return Skeletons_[SkeletonMap_[SkeletonName]];
	}
	else
	{
		LOG_ERROR("Skeleton not found: %s", SkeletonName.c_str());
		return MySTL::TSharedPtr<FSkeleton>();
	}
}

void MAnimationManager::PlayAnimation(std::string AnimationName, MySTL::TSharedPtr<FSkeleton> Skeleton)
{
	MySTL::TSharedPtr<FAnimation> Animation = GetAnimation(AnimationName);

	if (Animation.IsValid())
	{
		LOG_INFO("Playing animation: %s", AnimationName.c_str());
		Animation->SetSkeleton(Skeleton);
		PlayingAnimations_.push_back(Animation);
	}
}

void MAnimationManager::PlayAnimation(MySTL::TSharedPtr<FAnimation>& Animation, MySTL::TSharedPtr<FSkeleton> Skeleton)
{
	if (Animation.IsValid())
	{
		LOG_INFO("Playing animation");
		Animation->SetSkeleton(Skeleton);
		PlayingAnimations_.push_back(Animation);
	}
}

void MAnimationManager::StopAnimation(std::string AnimationName)
{
	MySTL::TSharedPtr<FAnimation> Animation = GetAnimation(AnimationName);
	if (Animation.IsValid())
	{
		PlayingAnimations_.Erase(Animation);
	}
}
