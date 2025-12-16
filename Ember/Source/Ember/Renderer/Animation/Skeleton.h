// Skeleton.h
// 动画骨架
// created by FirstRay2001, Dec/15/2025

#pragma once

#include <glm/glm.hpp>

struct aiScene;

namespace Ember
{
	class Bone;

	class Skeleton
	{
	public:
		void AddBone(Ref<Bone> bone);
		Ref<Bone> GetBone(const std::string& name);
		Ref<Bone> FindBone(const std::string& name);
		int FindIndex(const std::string& name);

		void Update();

		void SetUseInitialPose(bool value) { m_UseInitialPose = value; }

		const std::vector<glm::mat4>& GetBoneMatrices() const { return m_BoneMatrice; }

		void SetRootIndex(int rootIndex) { m_RootIndex = rootIndex; }

	public:
		static Ref<Skeleton> CreateFromAssimpScene_FBX(const aiScene* scene);

	private:
		int										m_RootIndex;
		std::unordered_map<std::string, int>	m_BoneIndexMap;
		std::vector<Ref<Bone>>					m_Bones;
		std::vector<glm::mat4>					m_BoneMatrice;
		bool									m_UseInitialPose = true;
	};
}
