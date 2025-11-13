// SceneHierarchy.h
// 场景层级面板
// created by FirstRay2001, Nov/13/2025

#pragma once

#include "Ember.h"
#include "Ember/Scene/Entity.h"
#include "Ember/Scene/Scene.h"

namespace Ember
{
	class SceneHierarchy
	{
	public:
		SceneHierarchy() = default;
		SceneHierarchy(const Ref<Scene>& scene);
		~SceneHierarchy() = default;

		// 设置当前场景上下文
		void SetContext(const Ref<Scene>& scene);

		// 渲染Hierarchy
		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
	};
}
