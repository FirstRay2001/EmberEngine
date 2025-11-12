// Renderer.cpp
// 渲染器类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Renderer.h"

namespace Ember
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_SceneData->CameraPosition = camera.GetPosition();
		s_SceneData->CameraDirection = camera.GetForwardDirection();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		// 设置相机矩阵
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetUniformMat4("u_Transform", transform);

		// 设置相机位置方向
		shader->Bind();
		shader->SetUniformFloat3("u_Camera.Position", s_SceneData->CameraPosition);
		shader->SetUniformFloat3("u_Camera.Direction", s_SceneData->CameraDirection);
		
		// 应用材质
		material->ApplyToShader(shader);

		// Draw
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Mesh& mesh, const glm::mat4& transform)
	{
		Submit(mesh.GetShader(), mesh.GetMaterial(), mesh.GetVertexArray(), transform);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(width, height);
	}
}