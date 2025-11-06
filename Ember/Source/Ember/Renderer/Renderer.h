// Renderer.h
// 渲染器类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Renderer/RenderCommand.h"
#include "Ember/Renderer/Camera.h"
#include "Ember/Renderer/Shader.h"

namespace Ember
{
	class Renderer
	{
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}


