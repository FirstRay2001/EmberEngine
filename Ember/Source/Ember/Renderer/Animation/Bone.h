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
		Bone(const std::string& name);

		void Update();

		void SetParent(Ref<Bone> parentBone);
		void AddChild(Ref<Bone> childBone)				{ m_Children.push_back(childBone); }
		std::string GetName() const						{ return m_Name; }
		void SetLocalTransform(const glm::mat4& local)	{ m_Local = local; }
		glm::mat4 GetLocalTransform() const				{ return m_Local; }
		void SetGlobalTransform(const glm::mat4& global) { m_Global = global; }
		glm::mat4 GetGlobalTransform() const			{ return m_Global; }
		glm::mat4 GetOffsetTransform() const			{ return m_Offset; }
		void SetOffsetTransform(const glm::mat4& offset) { m_Offset = offset; }
		glm::mat4 GetFinalTransform() const				{ return m_Final; }

	private:
		std::string				m_Name;
		glm::mat4				m_Local;
		glm::mat4				m_Global;
		glm::mat4				m_Offset;
		glm::mat4				m_Final;
		glm::mat4				m_Initial;
		Bone*					m_Parent;	// TODO: 使用弱引用
		std::vector<Ref<Bone>>	m_Children;
	};
}
