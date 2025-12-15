// Bone.cpp
// 骨骼类
// created by FirstRay2001, Dec/15/2025

#include "emberpch.h"
#include "Bone.h"

namespace Ember
{
	void Bone::Update(float deltaTime)
	{
		// 计算全局变换
		if (m_Parent)
		{
			m_Global = m_Parent->m_Global * m_Local;
		}
		// 根骨骼的本地变换即为全局变换
		else
		{
			m_Global = m_Local;
		}

		// 递归更新子骨骼
		for (auto& child : m_Children)
		{
			child->Update(deltaTime);
		}
	}
}