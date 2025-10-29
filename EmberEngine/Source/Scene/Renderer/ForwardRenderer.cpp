// ForwardRenderer.cpp
// 前向渲染管线
// Created by FirstRay2001, Oct/19/2025

#include "ForwardRenderer.h"
#include "Source/Scene/Actor/ModelActor.h"
#include "Source/Scene/Actor/CameraActor.h"
#include "Source/Scene/Actor/PointLightActor.h"
#include "Source/Scene/Actor/DirectionalLightActor.h"
#include "Source/Manager/ShaderManager.h"
#include "Source/Manager/TextureManager.h"
#include "Source/Manager/DebugDrawManager.h"
#include "Source/Scene/Component/Graphic/Shader.h"
#include "Source/Scene/Component/Graphic/Mesh.h"

FForwardRenderer::FForwardRenderer(unsigned int ScreenWidth, unsigned int ScreenHeight) :
	SCREEN_WIDTH(ScreenWidth),
	SCREEN_HEIGHT(ScreenHeight),
	SHADOW_WIDTH(1024),
	SHADOW_HEIGHT(1024),
	DirectionalShadowMapFBO_(),
	PointShadowMapFBO_(),
	SceneColorFBO_()
{
	Initialize();
}

FForwardRenderer::~FForwardRenderer()
{
	ClearSceneData();
}

void FForwardRenderer::Initialize()
{
	InitialzeShadowMap();
	InitializeOutline();
	InitializeSkybox();
	InitializePostProcess();
	InitializeDebug();
}

void FForwardRenderer::RenderScene()
{

	// DirectionalShadowMapFBO
	{
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, DirectionalShadowMapFBO_);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Directional Light Shadow Pass
		UpdateDirectionalLightShadowMaps();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	// PointShadowMapFBO
	{
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, PointShadowMapFBO_);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Point Light Shadow Pass
		UpdatePointLightShadowMaps();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	
	// 屏幕FBO
	{
		glBindFramebuffer(GL_FRAMEBUFFER, SceneColorFBO_);

		// 设置清屏颜色
		glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Skybox Pass
		RenderSkybox();

		// Base Pass
		RenderModels();

		// Outline Pass
		RenderOutline();

		// Debug Pass
		RenderDebug();

		// 清除缓冲区
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	// 默认帧缓冲区
	{
		// Post-Process Pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	// 解绑FBO，回到默认帧缓冲区

		// 清除缓冲区
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		PostProcess();

		// UI Pass
		RenderUI();
	}
}

void FForwardRenderer::Shutdown()
{
	// 清空场景数据
	ClearSceneData();
}

void FForwardRenderer::SetCurrentCamera(const MySTL::TSharedPtr<ACameraActor>& InCamera)
{
	CurrentCamera_ = InCamera;
}

void FForwardRenderer::SetDirectionalLight(const MySTL::TSharedPtr<ADirectionalLightActor>& InDirectionalLight)
{
	DirectionalLight_ = InDirectionalLight;
}

void FForwardRenderer::AddPointLight(const MySTL::TSharedPtr<APointLightActor>& InPointLight)
{
	PointLights_.push_back(InPointLight);
}

void FForwardRenderer::AddModel(const MySTL::TSharedPtr<AModelActor>& InModel)
{
	Models_.push_back(InModel);
}

void FForwardRenderer::ClearSceneData()
{
	CurrentCamera_.Reset();
	DirectionalLight_.Reset();
	SkyboxMesh_.Reset();
	PointLights_.Clear();
	Models_.Clear();

	// 删除FrameBuffer
	glDeleteFramebuffers(1, &DirectionalShadowMapFBO_);
	glDeleteFramebuffers(1, &SceneColorFBO_);
}

void FForwardRenderer::UpdateDirectionalLightShadowMaps()
{
	DisableStencilWrite();

	// 计算光源空间矩阵
	MyMath::FMatrix OrthographicProjection = MyMath::GetOrthoProjection(-10.0f, 10.0f, -10.0f, 10.0f, 
		CurrentCamera_->GetNearClip(), CurrentCamera_->GetFarClip());
	MyMath::FMatrix LightView = MyMath::GetLookAt(
		DirectionalLight_->GetDirection() * -10.0f,
		MyMath::FVector3(0.0f, 0.0f, 0.0f),
		MyMath::FVector3(0.0f, 1.0f, 0.0f)
	);
	DirectionalLightSpaceMatrix_ = OrthographicProjection * LightView;

	// 使用阴影贴图Shader
	auto ShadowMapShader = MShaderManager::GetInstance().GetShader("SimpleShadowMap");
	ShadowMapShader->Use();
	ShadowMapShader->SetMatrix("lightSpaceMatrix", DirectionalLightSpaceMatrix_.TransposeConst());

	// 绘制场景中的所有模型以生成阴影贴图
	for (int i = 0; i < Models_.Size(); i++)
	{
		auto ModelActor = Models_[i];

		// 应用骨骼动画
		ModelActor->SetBoneMatricesToShader(ShadowMapShader);

		ShadowMapShader->SetMatrix("model", ModelActor->GetWorldMatrix().Transpose());
		ModelActor->RawDraw();
	}
}

void FForwardRenderer::UpdatePointLightShadowMaps()
{
	// 仅在有点光源时才更新点光源阴影贴图
	if (PointLights_.Size() == 0)
		return;

	DisableStencilWrite();

	float Aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	float Near = 1.0f;
	float Far = PointLightShadowFarPlane_;

	MyMath::FMatrix Projection = MyMath::GetPerspectiveProjection(90.0f, Aspect, Near, Far);

	// 使用点光源阴影贴图Shader
	auto PointShadowMapShader = MShaderManager::GetInstance().GetShader("PointLightShadowMap");
	PointShadowMapShader->Use();

	// 创建6个方向的视图矩阵
	MyMath::FVector3 LightPos = PointLights_[0]->GetActorWorldLocation();
	MySTL::TVector<MyMath::FMatrix> ShadowTransforms;
	ShadowTransforms.push_back(Projection * 
		MyMath::GetLookAt(LightPos, LightPos + MyMath::FVector3(1.0f, 0.0f, 0.0f), MyMath::FVector3(0.0f, -1.0f, 0.0f)));  // Right
	ShadowTransforms.push_back(Projection * 
		MyMath::GetLookAt(LightPos, LightPos + MyMath::FVector3(-1.0f, 0.0f, 0.0f), MyMath::FVector3(0.0f, -1.0f, 0.0f))); // Left
	ShadowTransforms.push_back(Projection * 
		MyMath::GetLookAt(LightPos, LightPos + MyMath::FVector3(0.0f, 1.0f, 0.0f), MyMath::FVector3(0.0f, 0.0f, 1.0f)));   // Up
	ShadowTransforms.push_back(Projection * 
		MyMath::GetLookAt(LightPos, LightPos + MyMath::FVector3(0.0f, -1.0f, 0.0f), MyMath::FVector3(0.0f, 0.0f, -1.0f))); // Down
	ShadowTransforms.push_back(Projection * 
		MyMath::GetLookAt(LightPos, LightPos + MyMath::FVector3(0.0f, 0.0f, 1.0f), MyMath::FVector3(0.0f, -1.0f, 0.0f)));  // Front
	ShadowTransforms.push_back(Projection * 
		MyMath::GetLookAt(LightPos, LightPos + MyMath::FVector3(0.0f, 0.0f, -1.0f), MyMath::FVector3(0.0f, -1.0f, 0.0f))); // Back

	// 绘制场景中的所有模型以生成点光源阴影贴图
	for (int i = 0; i < 6; i++)
		PointShadowMapShader->SetMatrix(("shadowMatrices[" + std::to_string(i) + "]").c_str(), ShadowTransforms[i].TransposeConst());
	PointShadowMapShader->SetVector3("lightPos", LightPos);
	PointShadowMapShader->SetFloat("farPlane", Far);
	for (int i = 0; i < Models_.Size(); i++)
	{
		auto ModelActor = Models_[i];

		// 应用骨骼动画
		ModelActor->SetBoneMatricesToShader(PointShadowMapShader);

		PointShadowMapShader->SetMatrix("model", ModelActor->GetWorldMatrix().Transpose());
		ModelActor->RawDraw();
	}
}

void FForwardRenderer::RenderSkybox()
{
	// 禁用缓冲写入
	glDepthMask(GL_FALSE);
	glStencilMask(0x00);

	// 使用天空盒Shader
	MySTL::TWeakPtr<FShader> SkyboxShader = MShaderManager::GetInstance().GetShader("Skybox");
	SkyboxShader->Use();

	// 视图矩阵
	MyMath::FMatrix ViewMatrix = CurrentCamera_->GetWorldMatrix().Inverse();

	// 去除平移部分
	ViewMatrix(0, 3) = 0;
	ViewMatrix(1, 3) = 0;
	ViewMatrix(2, 3) = 0;

	// 投影矩阵
	MyMath::FMatrix ProjectionMatrix = CurrentCamera_->GetProjectionMatrix();

	// 设置矩阵
	SkyboxShader->SetMatrix("view", ViewMatrix.TransposeConst());
	SkyboxShader->SetMatrix("projection", ProjectionMatrix.TransposeConst());

	// 设置天空盒纹理
	MTextureManager::GetInstance().BindSamplerCube("Skybox", *SkyboxShader.Get(), "skybox");

	// 绘制天空盒
	SkyboxMesh_->Draw();

	// 启用缓冲写入
	glDepthMask(GL_TRUE);
}

void FForwardRenderer::RenderModels()
{
	glEnable(GL_DEPTH_TEST);	// 深度测试
	glEnable(GL_CULL_FACE);		// 面剔除
	glCullFace(GL_BACK);

	for (int i = 0; i < Models_.Size(); i++)
	{
		auto ModelActor = Models_[i];
		ModelActor->SetCamera(*CurrentCamera_);

		// 平行光
		if (DirectionalLight_.IsValid())
		{
			ModelActor->SetDirectionalLight(*DirectionalLight_.Get());
		}

		// TODO: 目前只支持一个点光源，后续支持多个点光源
		for (int j = 0; j < PointLights_.Size(); j++)
		{
			auto PointLightActor = PointLights_[j];
			ModelActor->SetPointLight(*PointLightActor);
			CurrentCamera_->RenderPointLight(PointLightActor.Get());
		}

		// 使用阴影贴图
		MTextureManager::GetInstance().BindSampler2D("DirectionalLightShadowMap", *ModelActor->GetShader(), "dirShadowMap");
		MTextureManager::GetInstance().BindSamplerCube("PointLightShadowMap", *ModelActor->GetShader(), "pointShadowMap");

		// 设置光源空间矩阵
		ModelActor->SetLightSpaceMatrix(DirectionalLightSpaceMatrix_);

		// 设置远平面距离
		ModelActor->GetShader()->SetFloat("farPlane", PointLightShadowFarPlane_);

		// 判断是否绘制轮廓
		if (ModelActor->IsDrawOutline())
			EnableStencilWrite();
		else
			DisableStencilWrite();

		// 绘制模型
		ModelActor->Draw();
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void FForwardRenderer::RenderOutline()
{
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	// 使用单色Shader
	auto SingleColorShader = MShaderManager::GetInstance().GetShader("SingleColor");
	SingleColorShader->Use();
	SingleColorShader->SetVector3("outlineColor", MyMath::FVector3(0.203, 0.596, 0.758));

	// 绘制轮廓
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	for (int i = 0; i < Models_.Size(); i++)
	{
		auto ModelActor = Models_[i];
		if (ModelActor->IsDrawOutline())
		{
			// 设置骨骼矩阵
			ModelActor->SetBoneMatricesToShader(SingleColorShader);

			// 放大一点，并稍微下移
			MyMath::FMatrix ScaleMatrix = MyMath::ToScaleMatrix(MyMath::FVector3(1.05f, 1.02f, 1.05f));
			MyMath::FMatrix TranslationMatrix = MyMath::ToTranslationMatrix(MyMath::FVector3(0,-0.015,0));
			MyMath::FMatrix ModelMatrix = TranslationMatrix * ModelActor->GetWorldMatrix() * ScaleMatrix;

			SingleColorShader->SetMatrix("model", ModelMatrix.Transpose());
			SingleColorShader->SetMatrix("view", CurrentCamera_->GetWorldMatrix().Inverse().Transpose());
			SingleColorShader->SetMatrix("projection", CurrentCamera_->GetProjectionMatrix().Transpose());
			ModelActor->RawDraw();
		}
	}

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
}

void FForwardRenderer::RenderDebug()
{
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	// 使用Normal Visualize Shader
	auto NormalVisualizeShader = MShaderManager::GetInstance().GetShader("NormalVisualize");
	NormalVisualizeShader->Use();
	NormalVisualizeShader->SetMatrix("view", CurrentCamera_->GetWorldMatrix().Inverse().TransposeConst());
	NormalVisualizeShader->SetMatrix("projection", CurrentCamera_->GetProjectionMatrix().TransposeConst());

	// 绘制法线
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	for (int i = 0; i < Models_.Size(); i++)
	{
		auto ModelActor = Models_[i];
		if (ModelActor->IsDrawNormal())
		{
			// 设置骨骼矩阵
			ModelActor->SetBoneMatricesToShader(NormalVisualizeShader);

			NormalVisualizeShader->SetMatrix("model", ModelActor->GetWorldMatrix().TransposeConst());
			ModelActor->RawDraw();
		}
	}

	// 使用Debug Line Shader
	auto DebugLineShader = MShaderManager::GetInstance().GetShader("DebugLine");
	DebugLineShader->Use();
	DebugLineShader->SetMatrix("view", CurrentCamera_->GetWorldMatrix().Inverse().TransposeConst());
	DebugLineShader->SetMatrix("projection", CurrentCamera_->GetProjectionMatrix().TransposeConst());

	// 绘制调试线
	MDebugDrawManager::GetInstance().DrawLines();

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
}

void FForwardRenderer::PostProcess()
{
	// 禁用深度测试
	glDisable(GL_DEPTH_TEST);

	// 模板测试
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0x00);

	// 使用后处理Shader
	auto PostProcessShader = MShaderManager::GetInstance().GetShader("PostProcess_ScreenColor");
	PostProcessShader->Use();

	// 绑定场景颜色纹理
	MTextureManager::GetInstance().BindSampler2D("SceneColor", *PostProcessShader.Get(), "screenTexture");

	// 绘制屏幕四边形
	ScreenQuadMesh_->Draw();

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
}

void FForwardRenderer::RenderUI()
{
}

void FForwardRenderer::EnableStencilWrite()
{
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
}

void FForwardRenderer::DisableStencilWrite()
{
	glStencilMask(0x00);
}

void FForwardRenderer::InitialzeShadowMap()
{
	// Diretional ShadowMap Shader
	MShaderManager::GetInstance().LoadShader("SimpleShadowMap", "ShadowMap/SimpleShadowMap.vert", "ShadowMap/SimpleShadowMap.frag");

	// 生成FrameBuffer
	glGenFramebuffers(1, &DirectionalShadowMapFBO_);

	// 阴影贴图大小
	SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	// 创建深度贴图
	unsigned int DirectionalLightShadowMapTex;
	glGenTextures(1, &DirectionalLightShadowMapTex);
	glBindTexture(GL_TEXTURE_2D, DirectionalLightShadowMapTex);

	// 绑定深度Component
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// 设置纹理参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 绑定FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, DirectionalShadowMapFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DirectionalLightShadowMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 储存纹理
	MTextureManager::GetInstance().StoreTexture("DirectionalLightShadowMap", DirectionalLightShadowMapTex);

	// Point Light ShadowMap Shader
	MShaderManager::GetInstance().LoadShader("PointLightShadowMap",
		"ShadowMap/PointShadowMap.vert",
		"ShadowMap/PointShadowMap.geom",
		"ShadowMap/PointShadowMap.frag");

	// 生成FrameBuffer
	glGenFramebuffers(1, &PointShadowMapFBO_);

	// 创建深度立方体贴图
	unsigned int PointLightShadowMapTex;
	glGenTextures(1, &PointLightShadowMapTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PointLightShadowMapTex);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	// 设置纹理参数
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// 绑定FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, PointShadowMapFBO_);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, PointLightShadowMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 储存纹理
	MTextureManager::GetInstance().StoreTexture("PointLightShadowMap", PointLightShadowMapTex);
}

void FForwardRenderer::InitializeOutline()
{
	// Single Color Shader
	MShaderManager::GetInstance().LoadShader("SingleColor", "SingleColor/SingleColor.vert", "SingleColor/SingleColor.frag");

	

	// 启用模板测试
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void FForwardRenderer::InitializeSkybox()
{
	MySTL::TVector<std::string> CubeTextures;
	CubeTextures.push_back("D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/Skybox2/right.jpg");
	CubeTextures.push_back("D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/Skybox2/left.jpg");
	CubeTextures.push_back("D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/Skybox2/top.jpg");
	CubeTextures.push_back("D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/Skybox2/bottom.jpg");
	CubeTextures.push_back("D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/Skybox2/front.jpg");
	CubeTextures.push_back("D:/Dev/Project/EmberEngine/EmberEngine/Resources/Texture/Skybox2/back.jpg");

	// 加载立方体贴图
	MTextureManager::GetInstance().LoadCubeTexture(CubeTextures, "Skybox");

	// 加载天空盒Shader
	MShaderManager::GetInstance().LoadShader("Skybox", "Skybox/Skybox.vert", "Skybox/Skybox.frag");

	// 创建天空盒网格
	SkyboxMesh_ = new FMesh(FMesh::CreateCube(1.0f));
}

void FForwardRenderer::InitializePostProcess()
{
	// Post-Process Shaders
	MShaderManager::GetInstance().LoadShader("PostProcess_ScreenColor", 
		"PostProcess/ScreenColor/ScreenColor.vert", "PostProcess/ScreenColor/ScreenColor.frag");

	// 创建屏幕四边形网格
	MySTL::TVector<FVertex> ScreenQuadVertices;
	ScreenQuadVertices.push_back(FMesh::CreateVertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	ScreenQuadVertices.push_back(FMesh::CreateVertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));
	ScreenQuadVertices.push_back(FMesh::CreateVertex(1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f));
	ScreenQuadVertices.push_back(FMesh::CreateVertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));

	MySTL::TVector<unsigned int> ScreenQuadIndices;
	ScreenQuadIndices.push_back(0);
	ScreenQuadIndices.push_back(1);
	ScreenQuadIndices.push_back(2);
	ScreenQuadIndices.push_back(0);
	ScreenQuadIndices.push_back(2);
	ScreenQuadIndices.push_back(3);

	ScreenQuadMesh_ = new FMesh(ScreenQuadVertices, ScreenQuadIndices);

	// 生成屏幕FBO
	glGenFramebuffers(1, &SceneColorFBO_);
	glBindFramebuffer(GL_FRAMEBUFFER, SceneColorFBO_);

	// 生成屏幕纹理
	unsigned int SceneColorTex;
	glGenTextures(1, &SceneColorTex);
	glBindTexture(GL_TEXTURE_2D, SceneColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	// 设置纹理参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 绑定到FrameBuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SceneColorTex, 0);

	// 存储纹理
	MTextureManager::GetInstance().StoreTexture("SceneColor", SceneColorTex);

	// 创建渲染缓冲对象用于深度和模板测试
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// 检查FrameBuffer是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Framebuffer is not complete!");
	}

	// 解绑FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FForwardRenderer::InitializeDebug()
{
	// Normal Visualize Shader
	MShaderManager::GetInstance().LoadShader("NormalVisualize",
		"Debug/NormalVisualize/NormalVisualize.vert", "Debug/NormalVisualize/NormalVisualize.geom", "Debug/NormalVisualize/NormalVisualize.frag");

	// Debug Line Shader
	MShaderManager::GetInstance().LoadShader("DebugLine",
		"Debug/DebugLine/DebugLine.vert", "Debug/DebugLine/DebugLine.frag");
}
