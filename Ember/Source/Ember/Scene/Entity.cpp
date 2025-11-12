// Entity.h
// ECS实体
// created by FirstRay2001, Nov/12/2025

#include "emberpch.h"
#include "Entity.h"

namespace Ember
{
	Entity::Entity(entt::entity handle, Scene* scene) :
		m_EntityHandle(handle),
		m_Scene(scene)
	{
	}
}
