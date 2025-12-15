// Skeleton.cpp
// 动画骨架
// created by FirstRay2001, Dec/15/2025

#include "emberpch.h"
#include "Skeleton.h"
#include "Bone.h"

namespace Ember
{
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

	void Skeleton::Update(float deltaTime)
	{
		
	}
}