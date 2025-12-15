// Animation.h
// 骨骼动画类
// created by FirstRay2001, Dec/15/2025

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Ember
{
	// 动画位置关键帧
	struct AnmiationPositonKeyframe
	{
		float TimeStamp;
		glm::vec3 Position;
	};

	// 动画旋转关键帧
	struct AnmiationRotationKeyframe
	{
		float TimeStamp;
		glm::quat Rotation;
	};

	// 动画缩放关键帧
	struct AnmiationScaleKeyframe
	{
		float TimeStamp;
		glm::vec3 Scale;
	};

	class Skeleton;

	class AnimationClip
	{
	public:
		AnimationClip(const std::string& boneName,
			const std::vector<AnmiationPositonKeyframe>& positionKeyframes,
			const std::vector<AnmiationRotationKeyframe>& rotationKeyframes,
			const std::vector<AnmiationScaleKeyframe>& scaleKeyframes) : 
			m_BoneName(boneName),
			m_PositionKeyframes(positionKeyframes),
			m_RotationKeyframes(rotationKeyframes),
			m_ScaleKeyframes(scaleKeyframes)
		{
		}
		const std::string& GetBoneName() const { return m_BoneName; }
		const std::vector<AnmiationPositonKeyframe>& GetPositionKeyframes() const { return m_PositionKeyframes; }
		const std::vector<AnmiationRotationKeyframe>& GetRotationKeyframes() const { return m_RotationKeyframes; }
		const std::vector<AnmiationScaleKeyframe>& GetScaleKeyframes() const { return m_ScaleKeyframes; }

	private:
		std::string								m_BoneName;
		std::vector<AnmiationPositonKeyframe>	m_PositionKeyframes;
		std::vector<AnmiationRotationKeyframe>	m_RotationKeyframes;
		std::vector<AnmiationScaleKeyframe>		m_ScaleKeyframes;
	};

	class Animation
	{
	public:
		Animation(const Ref<Skeleton>& skeleton, const std::vector<AnimationClip>& clips, float duration, float ticksPerSecond = 30.0f);

		void Update(float deltaTime);
	private:
		Ref<Skeleton>				m_Skeleton;
		std::vector<AnimationClip>	m_Clips;
		int							m_CurrentClipIndex = 0;
		float						m_CurrentTime = 0.0f;
		float						m_Duration;
		float						m_TicksPerSecond;
	};
}