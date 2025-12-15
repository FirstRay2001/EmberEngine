// Animation.cpp
// 骨骼动画类
// created by FirstRay2001, Dec/15/2025

#include "emberpch.h"
#include "Animation.h"
#include "Bone.h"
#include "Skeleton.h"

namespace Ember
{
	Animation::Animation(const Ref<Skeleton>& skeleton, const std::vector<AnimationClip>& clips, float duration, float ticksPerSecond) :
		m_Skeleton(skeleton),
		m_Clips(clips),
		m_Duration(duration),
		m_TicksPerSecond(ticksPerSecond)
	{
		m_CurrentClipIndex = 0;
		m_CurrentTime = 0.0f;
	}

	void Animation::Update(float deltaTime)
	{
		if (m_Clips.empty())
			return;

		// 更新当前时间
		m_CurrentTime += deltaTime * m_TicksPerSecond;
		m_CurrentTime = fmod(m_CurrentTime, m_Duration);

		// 更新骨骼变换
		for (int i = 0; i < m_Clips.size(); i++)
		{
			AnimationClip& clip = m_Clips[i];
			std::string boneName = clip.GetBoneName();
			Ref<Bone> bone = m_Skeleton->GetBone(boneName);

			if (!bone)
				continue;

			glm::mat4 localTransform(1.0f);

			// 缩放插值
			auto scaleKeyframes = clip.GetScaleKeyframes();
			if (scaleKeyframes.size() > 1)
			{
				for (int j = 0; j < scaleKeyframes.size() - 1; j++)
				{
					if (m_CurrentTime < scaleKeyframes[j + 1].TimeStamp)
					{
						float factor = (m_CurrentTime - scaleKeyframes[j].TimeStamp) / (scaleKeyframes[j + 1].TimeStamp - scaleKeyframes[j].TimeStamp);
						glm::vec3 scale = glm::mix(scaleKeyframes[j].Scale, scaleKeyframes[j + 1].Scale, factor);
						localTransform = glm::scale(localTransform, scale) * localTransform;
						break;
					}
				}
			}

			// 旋转插值
			auto rotationKeyframes = clip.GetRotationKeyframes();
			if (rotationKeyframes.size() > 1)
			{
				for (int j = 0; j < rotationKeyframes.size() - 1; j++)
				{
					if (m_CurrentTime < rotationKeyframes[j + 1].TimeStamp)
					{
						float factor = (m_CurrentTime - rotationKeyframes[j].TimeStamp) / (rotationKeyframes[j + 1].TimeStamp - rotationKeyframes[j].TimeStamp);
						glm::quat rotation = glm::slerp(rotationKeyframes[j].Rotation, rotationKeyframes[j + 1].Rotation, factor);
						localTransform = glm::mat4_cast(rotation) * localTransform;
						break;
					}
				}
			}

			// 位置插值
			auto positionKeyframes = clip.GetPositionKeyframes();
			if (positionKeyframes.size() > 1)
			{
				for (int j = 0; j < positionKeyframes.size() - 1; j++)
				{
					if (m_CurrentTime < positionKeyframes[j + 1].TimeStamp)
					{
						float factor = (m_CurrentTime - positionKeyframes[j].TimeStamp) / (positionKeyframes[j + 1].TimeStamp - positionKeyframes[j].TimeStamp);
						glm::vec3 position = glm::mix(positionKeyframes[j].Position, positionKeyframes[j + 1].Position, factor);
						localTransform = glm::translate(localTransform, position) * localTransform;
						break;
					}
				}
			}

			bone->SetLocalTransform(localTransform);
		}

		// 驱动骨骼
		m_Skeleton->Update(deltaTime);
	}
}