// Skeleton.h
// 动画骨架
// created by FirstRay2001, Dec/15/2025

#pragma once

namespace Ember
{
	class Bone;

	class Skeleton
	{
	public:
		void AddBone(Ref<Bone> bone);
		Ref<Bone> GetBone(const std::string& name);

		void Update(float deltaTime);

	private:
		std::unordered_map<std::string, int>	m_BoneIndexMap;
		std::vector<Ref<Bone>>					m_Bones;
	};
}
