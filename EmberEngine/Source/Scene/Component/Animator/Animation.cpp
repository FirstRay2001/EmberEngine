// Animation.cpp
// 骨骼动画类
// created by FirstRay2001, Oct/22/2025

#include "Animation.h"
#include "Source/Scene/Component/Animator/Skeleton.h"
#include "Source/Scene/Component/Animator/Bone.h"

FAnimation::FAnimation(float Duration) :
	Duration_(Duration)
{
}

void FAnimation::SetSkeleton(const MySTL::TSharedPtr<FSkeleton>& Skeleton)
{
	CurrentSkeleton_ = Skeleton;
	CurrentTime_ = 0.0f;
}

void FAnimation::AddAnimationClip(const FAnimationClip& Clip)
{
	Clips_.push_back(Clip);
}

void FAnimation::Tick(float DeltaTime)
{
	if (Clips_.Size() == 0)
		return;

	// 计算当前时间点
	CurrentTime_ += DeltaTime * TickPerSecond_ ;
	CurrentTime_ = fmod(CurrentTime_, Duration_);

	// 插值
	for (int i = 0; i < Clips_.Size(); i++)
	{
		FAnimationClip& Clip = Clips_[i];
		std::string TargetBoneName = Clip.GetTargetBoneName();
		MySTL::TSharedPtr<FBone> TargetBone = CurrentSkeleton_->GetBone(TargetBoneName);
		if (!TargetBone.IsValid())
			continue;

		MyMath::FMatrix ScaleMatrix, RotationMatrix, TranslationMatrix;

		// 缩放插值
		const MySTL::TVector<MyMath::FVector3>& ScaleKeyFrames = Clip.GetScaleKeyFrames();
		const MySTL::TVector<float>& ScaleTimeStamps = Clip.GetScaleTimeStamps();
		if (ScaleKeyFrames.Size() >= 2)
		{
			for (size_t j = 0; j < ScaleTimeStamps.Size() - 1; j++)
			{
				if (CurrentTime_ >= ScaleTimeStamps[j] && CurrentTime_ <= ScaleTimeStamps[j + 1])
				{
					float T = (CurrentTime_ - ScaleTimeStamps[j]) / (ScaleTimeStamps[j + 1] - ScaleTimeStamps[j]);
					MyMath::FVector3 NewScale = MyMath::Lerp(ScaleKeyFrames[j], ScaleKeyFrames[j + 1], T);
					MyMath::FMatrix LocalTransform = TargetBone->GetLocalTransform();
					ScaleMatrix = MyMath::ToScaleMatrix(NewScale);
					break;
				}
			}
		}

		// 旋转插值
		const MySTL::TVector<MyMath::Quaternion>& RotationKeyFrames = Clip.GetRotationKeyFrames();
		const MySTL::TVector<float>& RotationTimeStamps = Clip.GetRotationTimeStamps();
		if (RotationKeyFrames.Size() >= 2)
		{
			for (size_t j = 0; j < RotationTimeStamps.Size() - 1; j++)
			{
				if (CurrentTime_ >= RotationTimeStamps[j] && CurrentTime_ <= RotationTimeStamps[j + 1])
				{
					float T = (CurrentTime_ - RotationTimeStamps[j]) / (RotationTimeStamps[j + 1] - RotationTimeStamps[j]);
					MyMath::Quaternion NewRotation = MyMath::Slerp(RotationKeyFrames[j], RotationKeyFrames[j + 1], T);
					MyMath::FMatrix LocalTransform = TargetBone->GetLocalTransform();
					RotationMatrix = NewRotation.ToMatrix();
					break;
				}
			}
		}

		// 位置插值
		const MySTL::TVector<MyMath::FVector3>& PositionKeyFrames = Clip.GetPositionKeyFrames();
		const MySTL::TVector<float>& PositionTimeStamps = Clip.GetPositionTimeStamps();
		if (PositionKeyFrames.Size() >= 2)
		{
			for (size_t j = 0; j < PositionTimeStamps.Size() - 1; j++)
			{
				if (CurrentTime_ >= PositionTimeStamps[j] && CurrentTime_ <= PositionTimeStamps[j + 1])
				{
					float T = (CurrentTime_ - PositionTimeStamps[j]) / (PositionTimeStamps[j + 1] - PositionTimeStamps[j]);
					MyMath::FVector3 NewPosition = MyMath::Lerp(PositionKeyFrames[j], PositionKeyFrames[j + 1], T);
					MyMath::FMatrix LocalTransform = TargetBone->GetLocalTransform();
					TranslationMatrix = MyMath::ToTranslationMatrix(NewPosition);
					break;
				}
			}
		}

		//auto LocalTransform = TranslationMatrix * RotationMatrix * ScaleMatrix;
		auto LocalTransform = TranslationMatrix* RotationMatrix *ScaleMatrix;
		TargetBone->SetLocalTransform(LocalTransform);
	}

	// 更新骨骼位置
	CurrentSkeleton_->Tick(DeltaTime);
}

void FAnimationClip::SetTargetBoneName(const std::string& BoneName)
{
	TargetBoneName_ = BoneName;
}

std::string FAnimationClip::GetTargetBoneName() const
{
	return TargetBoneName_;
}

void FAnimationClip::AddPositionKeyFrame(const MyMath::FVector3& Position, float TimeStamp)
{
	PositionKeyFrames_.push_back(Position);
	PositionTimeStamps_.push_back(TimeStamp);
}

void FAnimationClip::AddRotationKeyFrame(const MyMath::Quaternion& Rotation, float TimeStamp)
{
	RotationKeyFrames_.push_back(Rotation);
	RotationTimeStamps_.push_back(TimeStamp);
}

void FAnimationClip::AddScaleKeyFrame(const MyMath::FVector3& Scale, float TimeStamp)
{
	ScaleKeyFrames_.push_back(Scale);
	ScaleTimeStamps_.push_back(TimeStamp);
}

const MySTL::TVector<MyMath::FVector3>& FAnimationClip::GetPositionKeyFrames() const
{
	return PositionKeyFrames_;
}

const MySTL::TVector<float>& FAnimationClip::GetPositionTimeStamps() const
{
	return PositionTimeStamps_;
}

const MySTL::TVector<MyMath::Quaternion>& FAnimationClip::GetRotationKeyFrames() const
{
	return RotationKeyFrames_;
}

const MySTL::TVector<float>& FAnimationClip::GetRotationTimeStamps() const
{
	return RotationTimeStamps_;
}

const MySTL::TVector<MyMath::FVector3>& FAnimationClip::GetScaleKeyFrames() const
{
	return ScaleKeyFrames_;
}

const MySTL::TVector<float>& FAnimationClip::GetScaleTimeStamps() const
{
	return ScaleTimeStamps_;
}
