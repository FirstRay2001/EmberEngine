// Skeleton.cpp
// 动画骨架
// created by FirstRay2001, Dec/15/2025

#include "emberpch.h"
#include "Skeleton.h"
#include "Bone.h"

#include "assimp_glm_helpers.h"
#include <assimp/scene.h>

namespace Ember
{
	namespace Utils
	{
		static void LoadMeshBones(aiMesh* mesh, Ref<Skeleton> dst)
		{
			for (unsigned int i = 0; i < mesh->mNumBones; i++)
			{
				aiBone* ai_bone = mesh->mBones[i];
				std::string boneName(ai_bone->mName.C_Str());
				Ref<Bone> bone = dst->FindBone(boneName);
				if (!bone)
				{
					bone = CreateRef<Bone>(boneName);
					dst->AddBone(bone);
					EMBER_CORE_INFO("Add Bone {0}, ID: {1}", boneName.c_str(), dst->FindIndex(boneName));
				}

				// 设置偏移矩阵
				glm::mat4 offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(
					ai_bone->mOffsetMatrix);

				// 提取缩放
				glm::vec3 scale = glm::vec3(
					glm::length(glm::vec3(offset[0])),
					glm::length(glm::vec3(offset[1])),
					glm::length(glm::vec3(offset[2]))
				);

				// 如果是接近10倍的统一缩放，移除它
				if (std::abs(scale.x - 10.0f) < 0.1f &&
					std::abs(scale.y - 10.0f) < 0.1f &&
					std::abs(scale.z - 10.0f) < 0.1f)
				{
					// 只移除缩放，保留旋转和平移
					offset[0] /= scale.x;
					offset[1] /= scale.y;
					offset[2] /= scale.z;
				}

				// 如果不是统一缩放，只归一化旋转部分
				else if (std::abs(scale.x - 1.0f) > 0.001f ||
					std::abs(scale.y - 1.0f) > 0.001f ||
					std::abs(scale.z - 1.0f) > 0.001f)
				{
					offset[0] = glm::normalize(glm::vec4(offset[0]));
					offset[1] = glm::normalize(glm::vec4(offset[1]));
					offset[2] = glm::normalize(glm::vec4(offset[2]));
				}

				bone->SetOffsetTransform(offset);
			}
		}

		static void LoadAllBones(aiNode* node, const aiScene* scene, Ref<Skeleton> dst)
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				LoadMeshBones(mesh, dst);
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				LoadAllBones(node->mChildren[i], scene, dst);
			}
		}

		static void SetupBoneHierarchy(aiNode* node, const aiScene* scene, Ref<Skeleton> dst, Ref<Bone> parentBone = nullptr)
		{
			std::string nodeName(node->mName.C_Str());
			Ref<Bone> bone = dst->GetBone(nodeName);
			if (bone)
			{
				if (parentBone)
				{
					bone->SetParent(parentBone);
					parentBone->AddChild(bone);
				}
			}

			parentBone = bone;

			// 递归处理子节点
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				SetupBoneHierarchy(node->mChildren[i], scene, dst, parentBone);
			}
		}
	}

	void Skeleton::AddBone(Ref<Bone> bone)
	{
		m_BoneIndexMap[bone->GetName()] = m_Bones.size();
		m_Bones.push_back(bone);
	}

	Ref<Bone> Skeleton::GetBone(const std::string& name)
	{
		auto it = m_BoneIndexMap.find(name);

		if (it == m_BoneIndexMap.end())
		{
			EMBER_CORE_WARN("Bone not found:{0}", name);
			return nullptr;
		}

		return m_Bones[it->second];
	}

	Ref<Bone> Skeleton::FindBone(const std::string& name)
	{
		auto it = m_BoneIndexMap.find(name);

		if (it == m_BoneIndexMap.end())
		{
			return nullptr;
		}

		return m_Bones[it->second];
	}

	int Skeleton::FindIndex(const std::string& name)
	{
		auto it = m_BoneIndexMap.find(name);
		if (it == m_BoneIndexMap.end())
		{
			return -1;
		}
		return it->second;
	}

	void Skeleton::Update()
	{
		m_BoneMatrice.clear();
		m_BoneMatrice.reserve(m_Bones.size());

		if (m_UseInitialPose)
		{
			for (auto& bone : m_Bones)
			{
				m_BoneMatrice.push_back(glm::mat4(1.0f));
			}
			return;
		}

		// 更新骨骼层次（计算全局变换）
		m_Bones[m_RootIndex]->Update();

		// 计算最终骨骼矩阵
		for (auto& bone : m_Bones)
		{
			glm::mat4 boneMatrix = bone->GetFinalTransform();
			m_BoneMatrice.push_back(boneMatrix);
		}
		
	}

	Ref<Skeleton> Skeleton::CreateFromAssimpScene_FBX(const aiScene* scene)
	{
		Ref<Skeleton> skeleton = CreateRef<Skeleton>();

		// TODO: 优化此处逻辑
		// 单独添加根骨骼
		Ref<Bone> rootBone = CreateRef<Bone>("Armature");
		rootBone->SetOffsetTransform(glm::mat4(1.0f));
		skeleton->AddBone(rootBone);

		// 加载所有骨骼
		Utils::LoadAllBones(scene->mRootNode, scene, skeleton);

		// 设置父子关系
		Utils::SetupBoneHierarchy(scene->mRootNode, scene, skeleton);

		

		return skeleton;
	}
}