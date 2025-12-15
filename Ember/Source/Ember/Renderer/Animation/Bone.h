// Bone.h
// 骨骼类
// created by FirstRay2001, Dec/15/2025

#pragma once
#include <glm/glm.hpp>

namespace Ember
{
	class Bone
	{
	public:
		void Update(float deltaTime);

		void SetParent(Ref<Bone> parentBone)			{ m_Parent = parentBone.get(); }
		void AddChild(Ref<Bone> childBone)				{ m_Children.push_back(childBone); }
		std::string GetName() const						{ return m_Name; }
		void SetLocalTransform(const glm::mat4& local)	{ m_Local = local; }
		glm::mat4 GetOffsetTransform() const			{ return m_Offset; }

	private:
		std::string				m_Name;
		glm::mat4				m_Local;
		glm::mat4				m_Global;
		glm::mat4				m_Offset;
		Bone*					m_Parent;	// TODO: 使用弱引用
		std::vector<Ref<Bone>>	m_Children;
	};
}
