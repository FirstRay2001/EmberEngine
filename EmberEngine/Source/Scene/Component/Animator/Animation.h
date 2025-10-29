// Animation.h
// ¹Ç÷À¶¯»­Àà
// created by FirstRay2001, Oct/22/2025

#pragma once

#include "Common.h"

class FAnimationClip
{
public:
	FAnimationClip() {};
	~FAnimationClip() {};

	void SetTargetBoneName(const std::string& BoneName);
	std::string GetTargetBoneName() const;
	void AddPositionKeyFrame(const MyMath::FVector3& Position, float TimeStamp);
	void AddRotationKeyFrame(const MyMath::Quaternion& Rotation, float TimeStamp);
	void AddScaleKeyFrame(const MyMath::FVector3& Scale, float TimeStamp);
	const MySTL::TVector<MyMath::FVector3>& GetPositionKeyFrames() const;
	const MySTL::TVector<float>& GetPositionTimeStamps() const;
	const MySTL::TVector<MyMath::Quaternion>& GetRotationKeyFrames() const;
	const MySTL::TVector<float>& GetRotationTimeStamps() const;
	const MySTL::TVector<MyMath::FVector3>& GetScaleKeyFrames() const;
	const MySTL::TVector<float>& GetScaleTimeStamps() const;

private:
	std::string							TargetBoneName_;
	MySTL::TVector<MyMath::FVector3>	PositionKeyFrames_;
	MySTL::TVector<float>				PositionTimeStamps_;
	MySTL::TVector<MyMath::Quaternion>	RotationKeyFrames_;
	MySTL::TVector<float>				RotationTimeStamps_;
	MySTL::TVector<MyMath::FVector3>	ScaleKeyFrames_;
	MySTL::TVector<float>				ScaleTimeStamps_;
};


class FAnimation
{
public:
	FAnimation(float Duration = 10.0f);
	~FAnimation() {};

	void SetDuration(float Duration) { Duration_ = Duration; }

public:
	void SetSkeleton(const MySTL::TSharedPtr<FSkeleton>& Skeleton);

	void AddAnimationClip(const FAnimationClip& Clip);

	void Tick(float DeltaTime);

private:
	MySTL::TWeakPtr<FSkeleton>		CurrentSkeleton_;
	MySTL::TVector<FAnimationClip>	Clips_;
	int								CurrentKeyFrameIndex_ = 0;
	float							CurrentTime_ = 0.0f;
	float							Duration_;
	int								TickPerSecond_ = 30;
};
