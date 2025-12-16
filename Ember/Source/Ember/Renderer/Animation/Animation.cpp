// Animation.cpp
// 骨骼动画类
// created by FirstRay2001, Dec/15/2025

#include "emberpch.h"
#include "Animation.h"
#include "Bone.h"
#include "Skeleton.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

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
		if (m_Clips.empty() || m_Skeleton == nullptr)
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
			glm::mat4 transMat, rotatMat, scaleMat;

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
						transMat = glm::translate(glm::mat4(1.0f), position);
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
						rotatMat = glm::mat4_cast(rotation);
						break;
					}
				}
			}

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
						scaleMat = glm::scale(glm::mat4(1.0f), scale);
						break;
					}
				}
			}

			bone->SetLocalTransform(transMat * rotatMat * scaleMat);
		}
	}

	Ref<Animation> Animation::LoadFromFBX(const std::filesystem::path& filepath)
	{
		// 读取场景
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			EMBER_CORE_ERROR("Failed to load FBX animation: {0}", importer.GetErrorString());
			return nullptr;
		}

		// 读取动画骨骼
		Ref<Skeleton> skeleton = Skeleton::CreateFromAssimpScene_FBX(scene);
		if (!skeleton)
		{
			EMBER_CORE_ERROR("Failed to create skeleton from FBX animation: {0}", filepath.string());
			return nullptr;
		}

		// 确保有动画
		if (scene->mNumAnimations < 1)
		{
			EMBER_CORE_WARN("No animations found in FBX file: {0}", filepath.string());
			return nullptr;
		}

		// 读取动画
		std::vector<AnimationClip> clips;
		aiAnimation* aiAnim = scene->mAnimations[0]; // 目前只支持第一个动画
		float duration = static_cast<float>(aiAnim->mDuration);
		float ticksPerSecond = static_cast<float>(aiAnim->mTicksPerSecond != 0 ? aiAnim->mTicksPerSecond : 30.0f);
		for (unsigned int i = 0; i < aiAnim->mNumChannels; i++)
		{
			aiNodeAnim* channel = aiAnim->mChannels[i];
			std::string boneName = channel->mNodeName.C_Str();

			// 位置关键帧
			std::vector<AnmiationPositonKeyframe> positionKeyframes;
			for (unsigned int j = 0; j < channel->mNumPositionKeys; j++)
			{
				AnmiationPositonKeyframe posKeyframe;
				posKeyframe.TimeStamp = static_cast<float>(channel->mPositionKeys[j].mTime);
				posKeyframe.Position = glm::vec3(
					channel->mPositionKeys[j].mValue.x,
					channel->mPositionKeys[j].mValue.y,
					channel->mPositionKeys[j].mValue.z
				);
				positionKeyframes.push_back(posKeyframe);
			}

			// 旋转关键帧
			std::vector<AnmiationRotationKeyframe> rotationKeyframes;
			for (unsigned int j = 0; j < channel->mNumRotationKeys; j++)
			{
				AnmiationRotationKeyframe rotKeyframe;
				rotKeyframe.TimeStamp = static_cast<float>(channel->mRotationKeys[j].mTime);
				rotKeyframe.Rotation = glm::quat(
					channel->mRotationKeys[j].mValue.w,
					channel->mRotationKeys[j].mValue.x,
					channel->mRotationKeys[j].mValue.y,
					channel->mRotationKeys[j].mValue.z
				);
				rotationKeyframes.push_back(rotKeyframe);
			}

			// 缩放关键帧
			std::vector<AnmiationScaleKeyframe> scaleKeyframes;
			for (unsigned int j = 0; j < channel->mNumScalingKeys; j++)
			{
				AnmiationScaleKeyframe scaleKeyframe;
				scaleKeyframe.TimeStamp = static_cast<float>(channel->mScalingKeys[j].mTime);
				scaleKeyframe.Scale = glm::vec3(
					channel->mScalingKeys[j].mValue.x,
					channel->mScalingKeys[j].mValue.y,
					channel->mScalingKeys[j].mValue.z
				);
				scaleKeyframes.push_back(scaleKeyframe);
			}
			AnimationClip clip(boneName, positionKeyframes, rotationKeyframes, scaleKeyframes);
			clips.push_back(clip);
		}

		Ref<Animation> animation = CreateRef<Animation>(skeleton, clips, duration, ticksPerSecond);
		return animation;
	}

	Ref<Animation> Animation::CreateFromFile(const std::filesystem::path& filepath)
	{
		// 获取文件扩展名
		auto extension = filepath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
		Ref<Animation> animation = nullptr;

		// FBX格式
		if (extension == ".fbx")
		{
			animation = LoadFromFBX(filepath);
			animation->m_Name = filepath.stem().string();
		}
		else
		{
			EMBER_CORE_ERROR("Unsupported animation format: {0}", extension);
		}

		return animation;
	}
}