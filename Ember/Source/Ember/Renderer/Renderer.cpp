// Renderer.cpp
// 渲染器类
// created by FirstRay2001, Nov/6/2025

#include "emberpch.h"
#include "Renderer.h"

namespace Ember
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;
	int Renderer::s_PointLightCount = 0;
	Ref<VertexArray> Renderer::s_DefaultCube = nullptr;

	void Renderer::Init()
	{
		RenderCommand::Init();

		// 创建默认立方体VAO
		s_DefaultCube = VertexArray::CreateCube(glm::vec3(1.0f));
	}

	void Renderer::BeginScene(Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_SceneData->CameraPosition = camera.GetPosition();
		s_SceneData->CameraDirection = camera.GetForwardDirection();
	}

	void Renderer::BeginScene(EditorCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_SceneData->CameraPosition = camera.GetPosition();
		s_SceneData->CameraDirection = camera.GetForwardDirection();
	}

	void Renderer::EndScene()
	{
		s_PointLightCount = 0;
		
		// TODO: 高效内存管理
		// 目前每次结束场景都重新分配内存，后续可以改为对象池等方式优化
		delete s_SceneData;
		s_SceneData = new SceneData();
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

		// 设置平行光
		shader->Bind();
		shader->SetUniformFloat3("u_DirectionalLight.Direction", s_SceneData->DirectionalLight.Direction);
		shader->SetUniformFloat3("u_DirectionalLight.Ambient", s_SceneData->DirectionalLight.Ambient);
		shader->SetUniformFloat3("u_DirectionalLight.Diffuse", s_SceneData->DirectionalLight.Diffuse);
		shader->SetUniformFloat3("u_DirectionalLight.Specular", s_SceneData->DirectionalLight.Specular);

		// 设置点光源
		shader->Bind();
		shader->SetUniformInt("u_PointLightCount", s_PointLightCount);
		for (int i = 0; i < s_PointLightCount; i++)
		{
			std::string index = std::to_string(i);
			shader->SetUniformFloat3("u_PointLights[" + index + "].Position", s_SceneData->PointLights[i].Position);
			shader->SetUniformFloat3("u_PointLights[" + index + "].Ambient", s_SceneData->PointLights[i].Ambient);
			shader->SetUniformFloat3("u_PointLights[" + index + "].Diffuse", s_SceneData->PointLights[i].Diffuse);
			shader->SetUniformFloat3("u_PointLights[" + index + "].Specular", s_SceneData->PointLights[i].Specular);
			shader->SetUniformFloat("u_PointLights[" + index + "].Constant", s_SceneData->PointLights[i].Constant);
			shader->SetUniformFloat("u_PointLights[" + index + "].Linear", s_SceneData->PointLights[i].Linear);
			shader->SetUniformFloat("u_PointLights[" + index + "].Quadratic", s_SceneData->PointLights[i].Quadratic);
		}
		
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

	void Renderer::Submit(const Mesh& mesh, int entityId, const glm::mat4& transform)
	{
		// 提交实体ID到Shader
		auto shader = mesh.GetShader();
		shader->Bind();
		shader->SetUniformInt("u_EntityID", entityId);

		Submit(mesh.GetShader(), mesh.GetMaterial(), mesh.GetVertexArray(), transform);
	}

	void Renderer::RenderSkybox(const Ref<Shader>& shader, const Ref<CubemapTexture>& cubemap, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		if (!cubemap || !shader)
			return;

		shader->Bind();

		// 去掉平移分量
		glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));
		shader->SetUniformMat4("u_View", view);
		shader->SetUniformMat4("u_Projection", projectionMatrix);
		cubemap->Bind(0);
		shader->SetUniformInt("u_Cubemap", 0);

		s_DefaultCube->Bind();
		RenderCommand::DrawIndexed(s_DefaultCube);
	}

	void Renderer::DrawLines(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetUniformMat4("u_Transform", transform);
		shader->SetUniformFloat3("u_Material.Albedo", glm::vec3(0.4f, 0.4f, 0.4f)); // 默认灰色
		vertexArray->Bind();
		RenderCommand::DrawLines(vertexArray);
	}

	void Renderer::AddPointLight(const PointLight& pointLight)
	{
		if (s_PointLightCount >= 4)
			return;
		s_SceneData->PointLights[s_PointLightCount] = pointLight;
		s_PointLightCount++;
	}

	void Renderer::AddDirectionalLight(const DirectionalLight& dirLight)
	{
		s_SceneData->DirectionalLight = dirLight;
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(width, height);
	}
}