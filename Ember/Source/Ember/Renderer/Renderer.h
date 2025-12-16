// Renderer.h
// 渲染器类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Renderer/RenderCommand.h"
#include "Ember/Renderer/Camera.h"
#include "Ember/Renderer/EditorCamera.h"
#include "Ember/Renderer/Shader.h"
#include "Ember/Renderer/Material.h"
#include "Ember/Renderer/Mesh.h"
#include "Ember/Renderer/Light.h"
#include "Ember/Renderer/Texture.h"

namespace Ember
{
	class Skeleton;

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(Camera& camera);
		static void BeginScene(EditorCamera& camera);
		static void EndScene();

		// 提交Shader VAO
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0));

		// 提交Shader 材质 VAO
		static void Submit(const Ref<Shader>& shader, const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0));

		static void Submit(const Mesh& mesh, const glm::mat4& transform = glm::mat4(1.0));

		// For Editor
		static void Submit(const Mesh& mesh, int entityId = -1, const glm::mat4& transform = glm::mat4(1.0));

		static void RenderSkybox(const Ref<Shader>& shader, const Ref<CubemapTexture>& cubemap, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

		static void DrawLines(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0));

		static void DrawOutline(const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0));

		static void AddPointLight(const PointLight& pointLight);
		static void AddDirectionalLight(const DirectionalLight& dirLight);

		// 绑定一个骨骼
		static void SetupSkeleton(Ref<Skeleton> skeleton);

		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::vec3 CameraPosition;
			glm::vec3 CameraDirection;
			glm::mat4 ViewProjectionMatrix;
			PointLight PointLights[4];
			DirectionalLight DirectionalLight;
			Ref<Skeleton> BindingSkeleton;
		};

		static int s_PointLightCount;
		static SceneData* s_SceneData;
		static Ref<VertexArray> s_DefaultCube;
	};
}


