// DebugRenderer.h
// Debug绘制
// created by FirstRay2001, Dec/16/2025

#pragma once

#include "Ember/Renderer/Renderer.h"
#include <glm/glm.hpp>

namespace Ember
{
	class VertexArray;

	class DebugRenderer
	{
	public:
		static DebugRenderer& Get()
		{
			static DebugRenderer instance;
			return instance;
		}

	public:
		DebugRenderer();
		void DrawLine(const glm::vec3& p0, const glm::vec3& p1);
		void FlushLines();
	private:
		Ref<Shader> m_Shader;
		std::vector<float> m_Points;
	};
}
