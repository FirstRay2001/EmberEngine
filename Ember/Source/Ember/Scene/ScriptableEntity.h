// ScriptableEntity.h
// 原生脚本实体
// created by FirstRay2001, Nov/13/2025

#pragma once

#include "Entity.h"
#include "Ember/Core/Timestep.h"

namespace Ember
{
	class ScriptableEntity
	{
		friend class Entity;
		friend class Scene;
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		template<typename T>
		T& As()
		{
			return static_cast<T&>(*this);
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep timestep) {}

	private:
		class Entity m_Entity;
	};
}

