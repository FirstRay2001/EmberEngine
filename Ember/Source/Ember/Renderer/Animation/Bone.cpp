// Bone.cpp
// 骨骼类
// created by FirstRay2001, Dec/15/2025

#include "emberpch.h"
#include "Bone.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Ember/Renderer/Debug/DebugRenderer.h"

namespace Ember
{
	Bone::Bone(const std::string& name) :
		m_Name(name),
		m_Parent(nullptr),
		m_Children()
	{
		m_Local = glm::mat4(1.0f);
		m_Global = glm::mat4(1.0f);
		m_Offset = glm::mat4(1.0f);
		m_Initial = glm::mat4(1.0f);
		m_Final = glm::mat4(1.0f);
	}

	void Bone::Update()
	{
		// 计算全局变换
		if (m_Parent)
		{
			m_Global = m_Parent->m_Global * m_Local;

			// Debug
			/*glm::vec3 parentPos = glm::vec3(m_Parent->m_Global[3]);
			glm::vec3 bonePos = glm::vec3(m_Global[3]);
			DebugRenderer::Get().DrawLine(parentPos, bonePos);*/
		}
		// 根骨骼的本地变换即为全局变换
		else
		{
			m_Global = glm::mat4(1.0f);
		}
		
		// 计算最终变换矩阵
		m_Final = m_Global * m_Offset;

		// 递归更新子骨骼
		for (auto& child : m_Children)
		{
			child->Update();
		}
	}

	void Bone::SetParent(Ref<Bone> parentBone)
	{
		m_Parent = parentBone.get();

		// 初始local
		m_Initial = glm::inverse(m_Parent->GetGlobalTransform()) * glm::inverse(m_Offset);
		m_Local = m_Initial;
		m_Global = m_Parent->GetGlobalTransform() * m_Local;
	}
}