// RendererAPI.h
// 渲染API类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include <glm/glm.hpp>
#include "Ember/Renderer/VertexArray.h"

namespace Ember
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL
		};

		enum class CullFace
		{
			None = 0,
			Back = 1,
			Front = 2
		};

	public:
		virtual void Init() = 0;
		virtual void* SetupMutiThread(void* window) = 0;
		virtual void InitMutiThread(void* window) = 0;
		virtual void ShutdownMutiThread() = 0;
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray) = 0;
		virtual void SetViewport(const uint32_t& width, const uint32_t& height) = 0;
		virtual void SetDepthMask(bool enabled) = 0;
		virtual void SetCullFace(CullFace face) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}
